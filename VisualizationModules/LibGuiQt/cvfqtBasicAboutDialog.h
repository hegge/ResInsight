//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2011-2012 Ceetron AS
//    
//   This library is free software: you can redistribute it and/or modify 
//   it under the terms of the GNU General Public License as published by 
//   the Free Software Foundation, either version 3 of the License, or 
//   (at your option) any later version. 
//    
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY 
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
//   FITNESS FOR A PARTICULAR PURPOSE.   
//    
//   See the GNU General Public License at <<http://www.gnu.org/licenses/gpl.html>> 
//   for more details. 
//
//##################################################################################################

#pragma once

#include <QtGui/QDialog>
#include <QtGui/QIcon>

class QGridLayout;


namespace cvfqt {


//==================================================================================================
//
// 
//
//==================================================================================================
class BasicAboutDialog : public QDialog
{
public:
    BasicAboutDialog(QWidget* parent);

    void	setApplicationName(const QString& appName);
    void	setApplicationVersion(const QString& ver);
    void	setCopyright(const QString& copyright);
    void    setApplicationIcon(const QIcon& icon);

    void	showCeeVizVersion(bool show);
    void	showQtVersion(bool show);
    void	addVersionEntry(const QString& verLabel, const QString& verText);
    void	setIsDebugBuild(bool isDebugBuild);

    void	create();

    QString openGLVersionString() const;

private:
    void	addStringPairToVerInfoLayout(const QString& labelStr, const QString& infoStr, QGridLayout* verInfoLayout, int insertRow);

private:
    bool			m_isCreated;		    // Indicates if the create() function has been called

    QString         m_appName;			    // Application name, appears in bold at the top of the dialog. 
    QString         m_appVersion;		    // Application version info. Can be empty
    QString			m_appCopyright;	        // Application copyright string. Can be empty
    QIcon           m_appIcon;

    bool			m_showCeeVizVersion;	// Flags whether CeeViz version info should be shown
    bool			m_showQtVersion;	    // Flags whether Qt version info should be shown
    QStringList     m_verLabels;		    // Labels for user specified version entries
    QStringList     m_verTexts;		        // The actual version text for user specified version entries

    bool			m_isDebugBuild;	        // If set to true, will show info in dlg to indicate that this is a debug build
};

}
