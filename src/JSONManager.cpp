#include "JSONManager.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

#include "JSONDocument.h"

QDX::JSONManager::JSONManager(QObject *parent): QObject(parent)
{
	this->addRequiredApplication(QApplication::applicationName());
}

bool QDX::JSONManager::save(const QString &filename, const QDX::JSONDocument &document)
{
	QFile file(filename);
	if (file.open(QIODevice::WriteOnly) == false) {
		errorOccurred(file.errorString());
		return false;
	}

	QJsonDocument converter(document.serialize());
	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	stream.setGenerateByteOrderMark(true);
	stream << converter.toJson();
	file.close();
	return true;
}

bool QDX::JSONManager::save(const QString &filename, const QJsonObject &data, const QString &type, const QString &name)
{
	JSONDocument document;
	document.setName(name);
	document.setType(type);
	document.setData(data);

	return this->save(filename, document);
}

bool QDX::JSONManager::save(const QString &filename, const QJsonObject &data, const QString &name)
{
	JSONDocument document;
	document.setName(name);
	document.setType(m_default_type);
	document.setData(data);

	return this->save(filename, document);
}

bool QDX::JSONManager::load(const QString &filename, JSONDocument &document) const
{
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly) == false) {
		errorOccurred(file.errorString());
		return false;
	}

	QJsonParseError error;
	QJsonDocument converter = QJsonDocument::fromJson(file.readAll(), &error);
	file.close();

	if (error.error != QJsonParseError::NoError) {
		errorOccurred(error.errorString());
		return false;
	}

	document.deserialize(converter.object());
	if (document.isValid() == false) {
		errorOccurred(QString("File is not a proper %1 JSON Document.").arg(this->displayName()));
		return false;
	}

	return this->validate(document);
}

bool QDX::JSONManager::load(const QString &filename, QJsonObject &data) const
{
	JSONDocument document;
	if (this->load(filename, document)) {
		data = document.data();
		return true;
	}
	return false;
}

QDX::JSONDocument QDX::JSONManager::loadDocument(const QString &filename) const
{
	JSONDocument document;
	this->load(filename, document);
	return document;
}

QString QDX::JSONManager::displayName() const
{
	return QApplication::applicationName() + " " + QApplication::applicationVersion();
}

QString QDX::JSONManager::displayRequiredApplications() const
{
	return m_required_applications.join(QStringLiteral(", "));
}

QString QDX::JSONManager::displayRequiredVersions() const
{
	QStringList result;
	for (const QString &version : m_required_versions) {
		QVector<QStringRef> range = version.splitRef(':');
		if (range.count() == 2) {
			result << QStringLiteral("%1 - %2").arg(range.at(0).toString()).arg(range.at(1).toString());
		} else {
			result << version;
		}
	}
	return result.join(QStringLiteral(", "));
}

QString QDX::JSONManager::displayRequiredTypes() const
{
	return m_required_types.join(QStringLiteral(", "));
}

bool QDX::JSONManager::validate(QDX::JSONDocument &document) const
{
	if (m_required_applications.isEmpty() == false) {
		bool check = false;
		for (const QString &application : m_required_applications) {
			if (document.application().compare(application, Qt::CaseInsensitive) == 0) {
				check = true;
				break;
			}
		}
		if (check == false) {
			errorOccurred(QStringLiteral("The file was not created in %1 application and cannot be opened.\nSupported applications: %2.")
				.arg(this->displayName()).arg(this->displayRequiredApplications()));
			return false;
		}
	}
	if (m_required_versions.isEmpty() == false) {
		bool check = false;
		bool only_range = true;
		for (const QString &version : m_required_versions) {
			QVector<QStringRef> range = version.splitRef(':');
			if (range.count() == 2) {
				check = document.compareVersion(range.at(0).toString()) >= 0 && document.compareVersion(range.at(1).toString()) <= 0;
			} else {
				check = document.checkVersion(version);
				only_range = false;
			}
			if (check) {
				break;
			}
		}
		if (check == false) {
			QString pattern = QStringLiteral("File version is not compatible.\n%1 requires %2");
			if (only_range == false) {
				pattern += QStringLiteral(" or newer.");
			}
			errorOccurred(pattern.arg(this->displayName()).arg(this->displayRequiredVersions()));
			return false;
		}
	}
	if (m_required_types.isEmpty() == false) {
		bool check = false;
		for (const QString &type : m_required_types) {
			if (document.type().compare(type, Qt::CaseInsensitive) == 0) {
				check = true;
				break;
			}
		}
		if (check == false) {
			errorOccurred(QStringLiteral("File type is not compatible.\n%1 requires %2.")
				.arg(this->displayName()).arg(this->displayRequiredTypes()));
			return false;
		}
	}
	return true;
}

QString QDX::JSONManager::defaultType() const
{
	return m_default_type;
}

void QDX::JSONManager::setDefaultType(const QString &default_type)
{
	m_default_type = default_type;
}

void QDX::JSONManager::setMandatoryType(const QString &type)
{
	m_default_type = type;
	this->addRequiredType(type);
}

QList<QString> QDX::JSONManager::requiredApplications() const
{
	return m_required_applications;
}

void QDX::JSONManager::setRequiredApplications(const QList<QString> &required_applications)
{
	m_required_applications = required_applications;
}

void QDX::JSONManager::addRequiredApplication(const QString &application)
{
	if (m_required_applications.contains(application) == false) {
		m_required_applications.append(application);
	}
}

QList<QString> QDX::JSONManager::requiredVersions() const
{
	return m_required_versions;
}

void QDX::JSONManager::setRequiredVersions(const QList<QString> &required_versions)
{
	m_required_versions = required_versions;
}

void QDX::JSONManager::addRequiredVersion(const QString &version)
{
	if (m_required_versions.contains(version) == false) {
		m_required_versions.append(version);
	}
}

QList<QString> QDX::JSONManager::requiredTypes() const
{
	return m_required_types;
}

void QDX::JSONManager::setRequiredTypes(const QList<QString> &required_types)
{
	m_required_types = required_types;
}

void QDX::JSONManager::addRequiredType(const QString &type)
{
	if (m_required_types.contains(type) == false) {
		m_required_types.append(type);
	}
}