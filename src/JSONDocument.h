#ifndef QDX_JSONDOCUMENT2_H
#define QDX_JSONDOCUMENT2_H

#include <QSharedPointer>
#include <QJsonObject>

namespace QDX {

	class JSONDocumentData;

	class JSONDocument
	{
	public:
		JSONDocument();
		JSONDocument(const JSONDocument &other);

		virtual QJsonObject serialize() const;
		virtual void deserialize(const QJsonObject &source);

		virtual void reset();

		bool isValid() const;

		QString name() const;
		void setName(const QString &name);

		QString description() const;
		void setDescription(const QString &description);

		QString application() const;
		void setApplication(const QString &application);

		QString type() const;
		void setType(const QString &type);

		QString version() const;
		void setVersion(const QString &version);

		QDateTime time() const;
		void setTime(const QDateTime &time);

		QJsonObject data() const;
		void setData(const QJsonObject &data);

		int compareVersion(const QString &other_version) const;
		bool checkVersion(const QString &required_version) const;

		static int compareVersions(const QString &version1, const QString &version2, bool strict = true);

	protected:
		virtual QString schema() const;
		virtual bool validateSource(const QJsonObject &source) const;
		virtual bool validateConsistency() const;

	private:
		QSharedDataPointer<JSONDocumentData> m_data;
	};

	class JSONDocumentData : public QSharedData
	{
	public:
		JSONDocumentData();

		bool valid = false;
		QString name;
		QString description;
		QString application;
		QString type;
		QString version;
		QDateTime time;
		QJsonObject data;
	};

} // namespace QDX

#endif // QDX_JSONDOCUMENT2_H
