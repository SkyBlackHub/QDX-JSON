#include "JSONDocument.h"

#include <QApplication>
#include <QCollator>

#include "JSONReadHelper.hpp"

QDX::JSONDocument::JSONDocument()
{
	m_data = new JSONDocumentData();
}

QDX::JSONDocument::JSONDocument(const QDX::JSONDocument &other): m_data(other.m_data)
{

}

QJsonObject QDX::JSONDocument::serialize() const
{
	QJsonObject result;

	result["schema"] = this->schema();

	result["application"] = m_data->application;
	result["type"] = m_data->type;
	result["version"] = m_data->version;
	result["time"] = m_data->time.toString(Qt::ISODate);

	if (m_data->name.isEmpty() == false) {
		result["name"] = m_data->name;
	}
	if (m_data->description.isEmpty() == false) {
		result["description"] = m_data->description;
	}

	result["data"] = m_data->data;

	return result;
}

void QDX::JSONDocument::deserialize(const QJsonObject &source)
{
	this->reset();

	if (this->validateSource(source) == false) {
		return;
	}

	JSONReadHelper helper(source);

	helper.read("application", m_data->application);
	helper.read("type", m_data->type);
	helper.read("version", m_data->version);
	helper.read("time", m_data->time);

	helper.read("name", m_data->name);
	helper.read("description", m_data->description);

	helper.read("data", m_data->data);

	m_data->valid = this->validateConsistency();
}

void QDX::JSONDocument::reset()
{
	m_data = new JSONDocumentData();
}

bool QDX::JSONDocument::isValid() const
{
	return m_data->valid;
}

QString QDX::JSONDocument::name() const
{
	return m_data->name;
}

void QDX::JSONDocument::setName(const QString &name)
{
	m_data->name = name;
}

QString QDX::JSONDocument::description() const
{
	return m_data->description;
}

void QDX::JSONDocument::setDescription(const QString &description)
{
	m_data->description = description;
}

QString QDX::JSONDocument::application() const
{
	return m_data->application;
}

void QDX::JSONDocument::setApplication(const QString &application)
{
	m_data->application = application;
}

QString QDX::JSONDocument::type() const
{
	return m_data->type;
}

void QDX::JSONDocument::setType(const QString &type)
{
	m_data->type = type;
}

QString QDX::JSONDocument::version() const
{
	return m_data->version;
}

void QDX::JSONDocument::setVersion(const QString &version)
{
	m_data->version = version;
}

QDateTime QDX::JSONDocument::time() const
{
	return m_data->time;
}

void QDX::JSONDocument::setTime(const QDateTime &time)
{
	m_data->time = time;
}

QJsonObject QDX::JSONDocument::data() const
{
	return m_data->data;
}

void QDX::JSONDocument::setData(const QJsonObject &data)
{
	m_data->data = data;
}

int QDX::JSONDocument::compareVersion(const QString &other_version) const
{
	return compareVersions(m_data->version, other_version);
}

bool QDX::JSONDocument::checkVersion(const QString &required_version) const
{
	return compareVersions(m_data->version, required_version) >= 0;
}

QString QDX::JSONDocument::schema() const
{
	return QStringLiteral("QDX::JSONDocument");
}

bool QDX::JSONDocument::validateSource(const QJsonObject &source) const
{
	if (source.contains("schema") == false) {
		return false;
	}
	if (source.value("schema").toString() != this->schema()) {
		return false;
	}
	return true;
}

bool QDX::JSONDocument::validateConsistency() const
{
	if (m_data->application.isEmpty()) {
		return false;
	}
	if (m_data->type.isEmpty()) {
		return false;
	}
	if (m_data->version.isEmpty()) {
		return false;
	}
	// version should not contain wildcards
	if (m_data->version.contains('*')) {
		return false;
	}
	return true;
}

int QDX::JSONDocument::compareVersions(const QString &version1, const QString &version2, bool strict)
{
	QRegularExpression splitter("\\.");
	QVector<QStringRef> split1 = version1.splitRef(splitter);
	QVector<QStringRef> split2 = version2.splitRef(splitter);

	QCollator collator;
	collator.setNumericMode(true);

	int index = 0;
	int wildcard = 0;
	for (const QStringRef &chunk1 : split1) {
		if (index >= split2.size()) {
			return wildcard == 2 ? 0 : 1;
		}
		const QStringRef &chunk2 = split2.at(index++);
		if (strict == false) {
			wildcard = 0;
			if (chunk1 == QChar('*'))	{
				wildcard = 1;
				continue;
			}
			if (chunk2 == QChar('*'))	{
				wildcard = 2;
				continue;
			}
		}
		bool ok1;
		bool ok2;
		int value1 = chunk1.toInt(&ok1);
		int value2 = chunk1.toInt(&ok2);
		if (ok1 && ok2 && value1 != value2) {
			return value1 > value2 ? 1 : -1;
		}
		int result = collator.compare(chunk1, chunk2);
		if (result != 0) {
			return result > 0 ? 1 : -1;
		}
	}

	if (wildcard != 1 && split2.size() > split1.size()) {
		return 1;
	}

	return 0;
}

QDX::JSONDocumentData::JSONDocumentData(): application(QApplication::applicationName()), version(QApplication::applicationVersion()), time(QDateTime::currentDateTime())
{

}
