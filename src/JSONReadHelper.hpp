#ifndef QDX_JSONREADHELPER_H
#define QDX_JSONREADHELPER_H

#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

namespace QDX {

	class JSONReadHelper
	{
	public:
		inline JSONReadHelper(const QJsonObject &object) : m_object(object) { }

		template <typename T>
		inline void read(const QString &key, T &variable) const
		{
			if (m_object.contains(key)) {
				QVariant value = m_object.value(key).toVariant();
				if (value.canConvert<T>()) {
					variable = value.value<T>();
				}
			}
		}

		inline void read(const QString &key, QJsonObject &variable) const
		{
			if (m_object.contains(key)) {
				QJsonValue value = m_object.value(key);
				if (value.type() == QJsonValue::Object) {
					variable = value.toObject();
				}
			}
		}

		inline void read(const QString &key, QJsonArray &variable) const
		{
			if (m_object.contains(key)) {
				QJsonValue value = m_object.value(key);
				if (value.type() == QJsonValue::Array) {
					variable = value.toArray();
				}
			}
		}

		inline void read(const QString &key, QString &variable) const
		{
			if (m_object.contains(key)) {
				QJsonValue value = m_object.value(key);
				if (value.type() == QJsonValue::String) {
					variable = value.toString();
				}
			}
		}

		template <typename T>
		inline void read(const QString &key, T &variable, const T &default_value) const
		{
			if (m_object.contains(key)) {
				QVariant value = m_object.value(key).toVariant();
				if (value.canConvert<T>()) {
					variable = value.value<T>();
				} else {
					variable = default_value;
				}
			} else {
				variable = default_value;
			}
		}

		inline void read(const QString &key, int &variable, int default_value) const
		{
			if (m_object.contains(key)) {
				variable = m_object.value(key).toInt(default_value);
			}
		}

		inline void read(const QString &key, bool &variable, bool default_value) const
		{
			if (m_object.contains(key)) {
				variable = m_object.value(key).toBool(default_value);
			}
		}

		inline void read(const QString &key, double &variable, double default_value) const
		{
			if (m_object.contains(key)) {
				variable = m_object.value(key).toDouble(default_value);
			}
		}

		inline void read(const QString &key, QString &variable, const QString &default_value) const
		{
			if (m_object.contains(key)) {
				variable = m_object.value(key).toString(default_value);
			}
		}

		inline void read(const QString &key, QJsonObject &variable, const QJsonObject &default_value) const
		{
			if (m_object.contains(key)) {
				variable = m_object.value(key).toObject(default_value);
			}
		}

		inline void read(const QString &key, QJsonArray &variable, const QJsonArray &default_value) const
		{
			if (m_object.contains(key)) {
				variable = m_object.value(key).toArray(default_value);
			}
		}

	private:
		const QJsonObject &m_object;
	};

} // namespace QDX

#endif // QDX_JSONREADHELPER_H