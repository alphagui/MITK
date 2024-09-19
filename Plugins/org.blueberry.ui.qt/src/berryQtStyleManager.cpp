/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "berryQtStyleManager.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStringList>
#include <QDirIterator>
#include <QFont>
#include <QFontDatabase>
#include <QIcon>

#include <berryLog.h>
#include <berryPlatformUI.h>
#include <berryQtPreferences.h>

#include "berryWorkbenchPlugin.h"

#include <mitkIPreferences.h>

namespace
{
  QString ParseColor(const QString& subject, const QString& pattern, const QString& fallback)
  {
    QRegularExpression re(pattern, QRegularExpression::CaseInsensitiveOption);
    auto match = re.match(subject);

    return match.hasMatch()
      ? match.captured(1)
      : fallback;
  }
}

namespace berry
{
QIcon QtStyleManager::ThemeIcon(const QByteArray &originalSVG)
{
  auto styleSheet = qApp->styleSheet();

  if (styleSheet.isEmpty())
    return QPixmap::fromImage(QImage::fromData(originalSVG));

  auto iconColor = ParseColor(styleSheet,
    QStringLiteral("iconColor\\s*[=:]\\s*(#[0-9a-f]{6})"),
    QStringLiteral("#000000"));

  auto iconAccentColor = ParseColor(styleSheet,
    QStringLiteral("iconAccentColor\\s*[=:]\\s*(#[0-9a-f]{6})"),
    QStringLiteral("#ffffff"));

  auto themedSVG = QString(originalSVG).replace(QStringLiteral("#00ff00"), iconColor, Qt::CaseInsensitive);
  themedSVG = themedSVG.replace(QStringLiteral("#ff00ff"), iconAccentColor, Qt::CaseInsensitive);

  return QPixmap::fromImage(QImage::fromData(themedSVG.toLatin1()));
}

QIcon QtStyleManager::ThemeIcon(const QString &resourcePath)
{
  QFile resourceFile(resourcePath);

  if (resourceFile.open(QIODevice::ReadOnly))
  {
    auto originalSVG = resourceFile.readAll();
    return ThemeIcon(originalSVG);
  }

  BERRY_WARN << "Could not read " << resourcePath;
  return QIcon();
}

QtStyleManager::QtStyleManager()
{
  AddDefaultStyle();
  AddDefaultFonts();
  ReadPreferences();
}

void QtStyleManager::ReadPreferences()
{
  auto* stylePref = WorkbenchPlugin::GetDefault()->GetPreferences()->Node(QtPreferences::QT_STYLES_NODE);

  QString paths = QString::fromStdString(stylePref->Get(QtPreferences::QT_STYLE_SEARCHPATHS, ""));
  QStringList pathList = paths.split(";", Qt::SkipEmptyParts);
  QStringListIterator it(pathList);
  while (it.hasNext())
  {
    AddStyles(it.next());
  }

  QString styleName = QString::fromStdString(stylePref->Get(QtPreferences::QT_STYLE_NAME, ""));
  // if a style is contributed via the Qt resource mechanism, it may not be
  // registered yet.
  if (Contains(styleName))
    SetStyle(styleName);
  else
    SetDefaultStyle();
}

QtStyleManager::~QtStyleManager()
{
  for (FileNameToStyleMap::const_iterator i = styles.begin(); i != styles.end(); ++i)
  {
    delete i.value();
  }
}

void QtStyleManager::AddDefaultStyle()
{
#ifndef _APPLE_
  AddStyle(":/org.blueberry.ui.qt/darkstyle.qss", "Dark");
  AddStyle(":/org.blueberry.ui.qt/lightstyle.qss", "Light");
  defaultStyle = styles[":/org.blueberry.ui.qt/darkstyle.qss"];
#endif
}

void QtStyleManager::AddDefaultFonts()
{
  m_customFontNames.append(QString("<<system>>"));

  this->AddFontFamily("Atkinson Hyperlegible");
  this->AddFontFamily("Fira Sans");
  this->AddFontFamily("Open Sans");
  this->AddFontFamily("Roboto");
}

void QtStyleManager::AddFontFamily(const QString& fontFamily)
{
  QString fontFamilyWithoutSpace = fontFamily;
  fontFamilyWithoutSpace.remove(QChar(' '));

  auto basePath = QString(":/org.blueberry.ui.qt/fonts/%1/%1-").arg(fontFamilyWithoutSpace);

  for (const auto& variant : { "Bold", "BoldItalic", "Italic", "Regular" })
    QFontDatabase::addApplicationFont(QString("%1%2.ttf").arg(basePath).arg(variant));

  m_customFontNames.push_back(fontFamily);
}

void QtStyleManager::ClearStyles()
{
  for (FileNameToStyleMap::iterator i = styles.begin(); i != styles.end(); )
  {
    if (!i.value()->fileName.startsWith(':'))
    {
      delete i.value();
      i = styles.erase(i);
    }
    else ++i;
  }
  SetDefaultStyle();
}

QtStyleManager::Style QtStyleManager::GetStyle() const
{
  return Style(currentStyle->name, currentStyle->fileName);
}

QString QtStyleManager::GetStylesheet() const
{
  return currentStyle->stylesheet;
}

QString QtStyleManager::GetActiveTabStylesheet() const
{
  return currentStyle->activeTabStylesheet;
}

QString QtStyleManager::GetTabStylesheet() const
{
  return currentStyle->tabStylesheet;
}

void QtStyleManager::AddStyle(const QString& styleFileName,
    const QString& styleName)
{
  auto newStyle = new ExtStyle();

  if (styleName.isEmpty())
  {
    QFileInfo info(styleFileName);
    newStyle->name = info.completeBaseName();
  }
  else
  {
    newStyle->name = styleName;
  }

  newStyle->fileName = styleFileName;

  styles.insert(newStyle->fileName, newStyle);
}

void QtStyleManager::GetFonts(QStringList& fontNames) const
{
  fontNames = m_customFontNames;
}

QString QtStyleManager::GetFont() const
{
  return m_currentFont;
}

void QtStyleManager::AddStyles(const QString& path)
{
  QDirIterator dirIt(path);
  while (dirIt.hasNext())
  {
    QString current = dirIt.next();
    QFileInfo info = dirIt.fileInfo();
    if (info.isFile() && info.isReadable())
    {
      QString fileName = info.fileName();
      if (fileName.endsWith("-tab.qss") || fileName.endsWith("-activetab.qss"))
        continue;

      if (fileName.endsWith(".qss"))
        AddStyle(current);
    }
  }
}

void QtStyleManager::ReadStyleData(ExtStyle* style)
{
  QString tabStyleFileName(style->fileName);
  QString activeTabStyleFileName(style->fileName);

  int index = style->fileName.lastIndexOf(".qss");
  tabStyleFileName.replace(index, 4, "-tab.qss");
  activeTabStyleFileName.replace(index, 4, "-activetab.qss");

  QFile styleFile(style->fileName);
  if (styleFile.open(QIODevice::ReadOnly))
  {
    QTextStream in(&styleFile);
    style->stylesheet = in.readAll();
  }
  else
  {
    BERRY_WARN << "Could not read " << style->fileName.toStdString();
  }

  QFile tabStyleFile(tabStyleFileName);
  if (tabStyleFile.open(QIODevice::ReadOnly))
  {
    QTextStream in(&tabStyleFile);
    style->tabStylesheet = in.readAll();
  }
  else
  {
    BERRY_WARN << "Could not read " << tabStyleFileName.toStdString();
  }

  QFile activeTabStyleFile(activeTabStyleFileName);
  if (activeTabStyleFile.open(QIODevice::ReadOnly))
  {
    QTextStream in(&activeTabStyleFile);
    style->activeTabStylesheet = in.readAll();
  }
  else
  {
    BERRY_WARN << "Could not read " << activeTabStyleFileName.toStdString();
  }
}

void QtStyleManager::RemoveStyle(const QString& styleFileName)
{
  if (currentStyle->fileName == styleFileName)
  {
    SetDefaultStyle();
  }

  delete styles.take(styleFileName);
}

void QtStyleManager::RemoveStyles(const QString& repo)
{
  if (repo.isEmpty())
  {
    ClearStyles();
    return;
  }

  for (FileNameToStyleMap::iterator i = styles.begin(); i != styles.end();)
  {
    ExtStyle* style = i.value();
    QFileInfo info(style->fileName);
    if (info.absolutePath() == repo)
    {
      if (style->name == currentStyle->name)
      {
        SetDefaultStyle();
      }

      i = styles.erase(i);
      delete style;
    }
    else
    {
      ++i;
    }
  }
}

void QtStyleManager::GetStyles(StyleList& styleNames) const
{
  for (FileNameToStyleMap::const_iterator i = styles.begin(); i != styles.end(); ++i)
    styleNames.push_back(Style(i.value()->name, i.value()->fileName));
}

void QtStyleManager::SetStyle(const QString& fileName)
{
  if (fileName.isEmpty())
  {
    SetDefaultStyle();
    return;
  }

  FileNameToStyleMap::const_iterator i = styles.find(fileName);

  ExtStyle* style = nullptr;
  if (i == styles.end())
  {
    BERRY_WARN << "Style " + fileName.toStdString() << " does not exist";
    style = defaultStyle;
  }
  else
  {
    style = i.value();
  }
  currentStyle = style;

  ReadStyleData(style);

  qApp->setStyleSheet(currentStyle->stylesheet);

  try
  {
    PlatformUI::GetWorkbench()->UpdateTheme();
  }
  catch (...)
  {
    // Swallow any exception if the Workbench instance has not been created yet.
    // Will be called later again but for now we just want to make sure that the
    // application style sheet can be at least already retrieved from qApp to
    // theme icons in plugins with eager activation policy.
  }
}

void QtStyleManager::SetFont(const QString& fontName)
{
  m_currentFont = fontName;
}

void QtStyleManager::SetFontSize(int fontSize)
{
  m_currentFontSize = fontSize;
}

void QtStyleManager::UpdateWorkbenchFont()
{
  if( m_currentFont == QString( "<<system>>" ) ||  m_currentFont == QString( "" ))
  {
    qApp->setFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont));
  }
  else
  {
    QFont font;
    font.setFamily(m_currentFont);
    font.setPointSize(m_currentFontSize);
    qApp->setFont(font);
  }
  qApp->setStyleSheet(currentStyle->stylesheet);
  PlatformUI::GetWorkbench()->UpdateTheme();
}

QtStyleManager::Style QtStyleManager::GetDefaultStyle() const
{
  return Style(defaultStyle->name, defaultStyle->fileName);
}

void QtStyleManager::SetDefaultStyle()
{
  SetStyle(defaultStyle->fileName);
}

bool QtStyleManager::Contains(const QString& fileName) const
{
  return styles.contains(fileName);
}

}
