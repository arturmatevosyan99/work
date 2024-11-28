#ifndef Cadabra_Queen_Bone_hpp
#define Cadabra_Queen_Bone_hpp

#include "Cupboard.hpp"
#include "Skeleton.hpp"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QPair>
#include <QVariant>
#include <QCommandLineOption>
#include <QQmlEngine>
#include <QQmlContext>
#include <QFile>

namespace Cadabra { namespace Queen {
	class Bone : public QObject
	{
	friend class Skeleton;

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using OptionMap = QMap<QString, QPair<QCommandLineOption, QVariant>>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Bone(const QString& name = "");
		Bone(const Bone& bone);
		virtual ~Bone();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */	
	private:
		const QString _name;
		Q_PROPERTY(QString name READ getName CONSTANT)

		OptionMap _option_map;
		Cupboard* _cupboard;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		QVariant& optCommandLine(const QString& option_name, const QStringList& name_list, const QString& description, const QString& value_name = QString(), const QVariant& default_value = QVariant());

	public:
		virtual bool registerMetaObject();
		virtual bool registerMetaType();
		virtual bool registerMetaTypeConverter();
		virtual bool registerMetaTypeStreamOperator();
		virtual bool registerQMLType();
		virtual bool registerAceAction();
		virtual bool registerAceScheme();
		virtual bool registerQMLSingletonType();
		virtual bool registerTestCase();
		virtual bool tweakApplicationEngine(QQmlApplicationEngine& engine);
		virtual bool tweakRootContext(QQmlContext& context);
		virtual bool tweakRootObject(QObject& object);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getName() const;

		Bone::OptionMap& getCommandLineOptionMap();
		const Bone::OptionMap& getCommandLineOptionMap() const;

		Cupboard& getCupboard();
		const Cupboard& getCupboard() const;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QString makeTitle() const;
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onAttach(Skeleton& skeleton);
		virtual void onDetach(Skeleton& skeleton);
		virtual void onPreDisclose(Cupboard& cupboard);
		virtual void onPostDisclose(Cupboard& cupboard);
	/** @} */
	};
} }

Q_DECLARE_METATYPE(Cadabra::Queen::Bone)

#endif
