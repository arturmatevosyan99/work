/**
 @file AbstractJob.cpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Repository_Model_AbstractJob_hpp
#define Cadabra_Queen_Repository_Model_AbstractJob_hpp

#include "../Meta/Job.hpp"

#include <QObject>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Session;
	class AbstractJob : public QObject
	{
	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::JobState state READ getState NOTIFY stateChanged)
//		Q_PROPERTY(QString errorMessage READ getError NOTIFY )
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractJob(const Meta::Job::Scheme scheme, QObject* parent = nullptr);
		virtual ~AbstractJob() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const Meta::Job::Scheme _scheme;
		Meta::Job::Id _id;
		JobState _state;
		Session* _session;
		QString _error;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void plan(Session* session);
		bool probe();
		void fault(const QString& message);

	protected:
		void complete(bool yesno = true);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QJsonObject makeEnvironment() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Meta::Job::Scheme& getScheme() const;
		const JobState& getState() const;
		const QString& getError() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void stateChanged();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
