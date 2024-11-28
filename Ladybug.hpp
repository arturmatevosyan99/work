#ifndef Cadabra_Queen_Ladybug_hpp
#define Cadabra_Queen_Ladybug_hpp

#include "Skeleton.hpp"

#include "Stress/Stress.hpp"

#include <QtTest/QtTest>

namespace Cadabra { namespace Queen {
	class Ladybug : public Skeleton
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Ladybug(int argc = 0);
		Ladybug(int& argc, char** argv, int flags = ApplicationFlags);
		virtual ~Ladybug() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QPointer<Stress::Extension> _stress_extension;

		const QCommandLineOption _model_repository_option;
		const QCommandLineOption _model_mask_option;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		bool registerTestCase();

	protected:
		virtual bool config() override;
		virtual bool tune() override;
		virtual bool extend() override;
		virtual int boot() override;
		virtual int command() override;
		virtual int command(const QStringList& path_list);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void testCoreUnit();
		void testCoreIntegration();
		void testCoreSystem();
		void testCoreDeath();
		void testCoreProof();
		void testCoreDevelopment();
		void testCoreCustom();

		void testStressUnit();
		void testStressIntegration();
		void testStressSystem();
		void testStressDeath();
		void testStressProof();
		void testStressDevelopment();
		void testStressCustom();

		void testRepositoryUnit();
		void testRepositoryIntegration();
		void testRepositorySystem();
		void testRepositoryDeath();
		void testRepositoryProof();
		void testRepositoryDevelopment();
		void testRepositoryCustom();

		void testConversationUnit();
		void testConversationIntegration();
		void testConversationSystem();
		void testConversationDeath();
		void testConversationProof();
		void testConversationDevelopment();
		void testConversationCustom();

		void testERPUnit();
		void testERPIntegration();
		void testERPSystem();
		void testERPDeath();
		void testERPProof();
		void testERPDevelopment();
		void testERPCustom();

		void testSDUnit();
		void testSDIntegration();
		void testSDSystem();
		void testSDDeath();
		void testSDProof();
		void testSDDevelopment();
		void testSDCustom();

		void testECUnit();
		void testECIntegration();
		void testECSystem();
		void testECDeath();
		void testECProof();
		void testECDevelopment();
		void testECCustom();

		void testMentalgamiUnit();
		void testMentalgamiIntegration();
		void testMentalgamiSystem();
		void testMentalgamiDeath();
		void testMentalgamiProof();
		void testMentalgamiDevelopment();
		void testMentalgamiCustom();

		void testDrOblozhkoUnit();
		void testDrOblozhkoIntegration();
		void testDrOblozhkoSystem();
		void testDrOblozhkoDeath();
		void testDrOblozhkoProof();
		void testDrOblozhkoDevelopment();
		void testDrOblozhkoCustom();
	/** @} */
	};
} }

#endif
