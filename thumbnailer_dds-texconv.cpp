/*  SPDX-FileCopyrightText: 2022 Mathieu Eyraud
	SPDX-FileCopyrightText: 2026 failsafe42
	SPDX-License-Identifier: GPL-2.0-or-later

	https://github.com/failsafe42/dds-texconv-thumbnailer-kde

	dds-texconv-thumbnailer-kde
	Copyright (C) 2026 failsafe42

	Based on dds10-thumbnailer-kde by Mathieu Eyraud:
	https://github.com/meyraud705/dds10-thumbnailer-kde

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "texconv.h"

#include <QtCore/QTemporaryDir>
#include <QtCore/QFileInfo>
#include <QtGui/QImage>

#include <KPluginFactory>
#include <kio/thumbnailcreator.h>

class DDSCreator : public KIO::ThumbnailCreator {
		Q_OBJECT
	public:
		DDSCreator(QObject *parent, const QVariantList &args);
		virtual ~DDSCreator() = default;
		
		KIO::ThumbnailResult create(const KIO::ThumbnailRequest &request) override;
};

K_PLUGIN_CLASS_WITH_JSON(DDSCreator, "thumbnailer_dds-texconv.json")

DDSCreator::DDSCreator(QObject *parent, const QVariantList &args) : KIO::ThumbnailCreator(parent, args) {
}

bool isSRGB(uint32_t format){
	switch(format){
		case 29:
    	case 72:
		case 75:
		case 78:
		case 91:
		case 93:
		case 99:
			return true;
		default:
			return false;
	}
}

// Thumbnailer /////////////////////////////////////////////////////////////////
KIO::ThumbnailResult DDSCreator::create(const KIO::ThumbnailRequest &request) {
	QFileInfo file_dds(request.url().toLocalFile());
	
	// create unique temporary directory
	QTemporaryDir file_png_dir;

	// get paths for texconv
	std::wstring dds_path_w, png_dir_w;
	dds_path_w = file_dds.canonicalFilePath().toStdWString();
	png_dir_w = file_png_dir.path().toStdWString();
	
	TexMetadata info;
	wchar_t* diagargs[] = {L"info", L"--", dds_path_w.data()};
	texdiag(sizeof(diagargs) / sizeof(diagargs[0]), diagargs, info);

	wchar_t* format = L"R8G8B8A8_UNORM";
	if(isSRGB(info.format)){
		format = L"R8G8B8A8_UNORM_SRGB";
	}

	// convert to png
	wchar_t* convargs[] = {L"-ft", L"png", L"-f", format, L"-o", png_dir_w.data(), L"--", dds_path_w.data()};
	texconv(sizeof(convargs) / sizeof(convargs[0]), convargs);

	// fill the QImage
	QImage img(file_png_dir.path()+QLatin1Char('/')+file_dds.baseName()+QStringLiteral(".png"));

	return KIO::ThumbnailResult::pass(img);
}

#include "thumbnailer_dds-texconv.moc"
