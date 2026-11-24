/*  SPDX-FileCopyrightText: 2022 Mathieu Eyraud 
	SPDX-License-Identifier: GPL-2.0-or-later
	
	https://github.com/meyraud705/dds10-thumbnailer-kde
	
	dds10-thumbnailer-kde
	Copyright (C) 2022 Mathieu Eyraud

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

#include <string>
#include <sys/random.h>
#include "texconv.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtGui/QImage>
#include <QtCore/QDebug>

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

QString genrandom() {
	std::string out;
	for (int i = 0; i < 5; ++i){
		u_int8_t randnum;
		getrandom(&randnum, sizeof(randnum), 0);
		char c = (randnum % 52) + 65;
		if (c > 90) c += 6;
		out += c;
	}
	return QString::fromStdString(out);
}

// Thumbnailer /////////////////////////////////////////////////////////////////
KIO::ThumbnailResult DDSCreator::create(const KIO::ThumbnailRequest &request) {
	QFileInfo file_dds(request.url().toLocalFile());
	
	// create unique temporary directory
	QString temp_dir_prefix = QStringLiteral("/tmp/texconvthumbnailer-");
	QDir file_png_dir(temp_dir_prefix+genrandom());
	while (file_png_dir.exists()){
		file_png_dir.setPath(temp_dir_prefix+genrandom());
	}
	file_png_dir.mkpath(QStringLiteral("."));

	// get paths for texconv
	std::wstring dds_path_w, png_dir_w;
	dds_path_w = file_dds.canonicalFilePath().toStdWString();
	png_dir_w = file_png_dir.canonicalPath().toStdWString();

	// convert to png
	wchar_t* argv[] = {L"-ft", L"png", L"-f", L"rgba", L"-srgbo", L"-o", png_dir_w.data(), L"--", dds_path_w.data()};
	texconv(sizeof(argv) / sizeof(argv[0]), argv);

	// fill the QImage
	QImage img(file_png_dir.canonicalPath()+QLatin1Char('/')+file_dds.baseName()+QStringLiteral(".png"));

	// delete temporary directory
	file_png_dir.removeRecursively();

	return KIO::ThumbnailResult::pass(img);
}

#include "thumbnailer_dds-texconv.moc"
