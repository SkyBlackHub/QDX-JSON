#ifndef QDX_JSONMANAGER_H
#define QDX_JSONMANAGER_H

#include <QObject>

namespace QDX {

	class JSONDocument;

	class JSONManager : public QObject
	{
		Q_OBJECT
	public:
		explicit JSONManager(QObject *parent = nullptr);

		bool save(const QString &filename, const JSONDocument &document);
		bool save(const QString &filename, const QJsonObject &data, const QString &type, const QString &name = QString());
		bool save(const QString &filename, const QJsonObject &data, const QString &name = QString());

		bool load(const QString &filename, JSONDocument &document) const;
		bool load(const QString &filename, QJsonObject &data) const;

		JSONDocument loadDocument(const QString &filename) const;

		QString defaultType() const;
		void setDefaultType(const QString &default_type);
		void setMandatoryType(const QString &type);

		QList<QString> requiredApplications() const;
		void setRequiredApplications(const QList<QString> &required_applications);
		void addRequiredApplication(const QString &application);

		QList<QString> requiredVersions() const;
		void setRequiredVersions(const QList<QString> &required_versions);
		void addRequiredVersion(const QString &version);

		QList<QString> requiredTypes() const;
		void setRequiredTypes(const QList<QString> &required_types);
		void addRequiredType(const QString &type);

	protected:
		virtual QString displayName() const;

		virtual QString displayRequiredApplications() const;
		virtual QString displayRequiredVersions() const;
		virtual QString displayRequiredTypes() const;

		virtual bool validate(JSONDocument &document) const;

	signals:
		void errorOccurred(const QString &message) const;

	private:
		QString m_default_type;

		QList<QString> m_required_applications;
		QList<QString> m_required_versions;
		QList<QString> m_required_types;
	};

} // namespace QDX

#endif // QDX_JSONMANAGER_H
