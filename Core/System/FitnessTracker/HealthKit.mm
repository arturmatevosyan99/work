#include "../FitnessTracker.hpp"

#import <HealthKit/HealthKit.h>

#include <QString>
#include <QSettings>
#include <QScopedPointer>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class HealthKit : public FitnessTracker
	{
	public:
		HealthKit()
		:
			FitnessTracker(),
			_store(nullptr)
		{
			if (isAvaliable()) {
				_store = [[HKHealthStore alloc] init];
			}
/*
			// Выделяем память, заполненную нулями
			void *newObject = calloc(1, class_getInstanceSize([TestObject class]));
			// Задаём isa прямой записью в память
			Class *c = (Class *)newObject;
			c[0] = [TestObject class];
			// Здесь __bridge_transfer-каст нужен для передачи объекта в ARC - иначе утечёт
			obj = (__bridge_transfer TestObject *)newObject;
			// Посылаем init - объект готов!
			obj = [obj init];
*/
		}

		virtual ~HealthKit() override
		{
			if (_store) {
				[_store dealloc];
			}
		}

	private:
		HKHealthStore* _store;

	private:
		bool request(void (^completion_block)(BOOL, NSError* _nullable))
		{
			if (!hasStore()) {
				return false;
			}

			NSSet* identifier_set = [[NSSet alloc] initWithObjects:
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBodyMassIndex],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBodyFatPercentage],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierHeight],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBodyMass],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierLeanBodyMass],
	/*
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierWaistCircumference],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierStepCount],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDistanceWalkingRunning],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDistanceCycling],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDistanceWheelchair],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBasalEnergyBurned],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierActiveEnergyBurned],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierFlightsClimbed],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierNikeFuel],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierAppleExerciseTime],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierPushCount],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDistanceSwimming],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierSwimmingStrokeCount],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierVO2Max],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDistanceDownhillSnowSports],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierHeartRate],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBodyTemperature],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBasalBodyTemperature],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBloodPressureSystolic],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBloodPressureDiastolic],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierRespiratoryRate],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierRestingHeartRate],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierWalkingHeartRateAverage],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierHeartRateVariabilitySDNN],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierOxygenSaturation],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierPeripheralPerfusionIndex],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBloodGlucose],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierNumberOfTimesFallen],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierElectrodermalActivity],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierInhalerUsage],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierInsulinDelivery],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierBloodAlcoholContent],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierForcedVitalCapacity],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierForcedExpiratoryVolume1],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierPeakExpiratoryFlowRate],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryFatTotal],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryFatPolyunsaturated],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryFatMonounsaturated],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryFatSaturated],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryCholesterol],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietarySodium],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryCarbohydrates],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryFiber],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietarySugar],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryEnergyConsumed],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryProtein],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryVitaminA],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryVitaminB6],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryVitaminB12],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryVitaminC],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryVitaminD],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryVitaminE],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryVitaminK],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryCalcium],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryIron],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryThiamin],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryRiboflavin],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryNiacin],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryFolate],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryBiotin],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryPantothenicAcid],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryPhosphorus],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryIodine],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryMagnesium],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryZinc],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietarySelenium],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryCopper],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryManganese],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryChromium],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryMolybdenum],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryChloride],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryPotassium],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryCaffeine],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierDietaryWater],
																	[HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierUVExposure],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierSleepAnalysis],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierAppleStandHour],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierCervicalMucusQuality],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierOvulationTestResult],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierMenstrualFlow],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierIntermenstrualBleeding],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierSexualActivity],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierMindfulSession],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierHighHeartRateEvent],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierLowHeartRateEvent],
																	[HKQuantityType quantityTypeForIdentifier:HKCategoryTypeIdentifierIrregularHeartRhythmEvent],
																	[HKQuantityType quantityTypeForIdentifier:HKCharacteristicTypeIdentifierBiologicalSex],
																	[HKQuantityType quantityTypeForIdentifier:HKCharacteristicTypeIdentifierBloodType],
																	[HKQuantityType quantityTypeForIdentifier:HKCharacteristicTypeIdentifierDateOfBirth],
																	[HKQuantityType quantityTypeForIdentifier:HKCharacteristicTypeIdentifierFitzpatrickSkinType],
																	[HKQuantityType quantityTypeForIdentifier:HKCharacteristicTypeIdentifierWheelchairUse],
																	[HKQuantityType quantityTypeForIdentifier:HKCorrelationTypeIdentifierBloodPressure],
																	[HKQuantityType quantityTypeForIdentifier:HKCorrelationTypeIdentifierFood],
																	[HKQuantityType quantityTypeForIdentifier:HKDocumentTypeIdentifierCDA],
																	[HKQuantityType quantityTypeForIdentifier:HKWorkoutTypeIdentifier],
																	[HKQuantityType quantityTypeForIdentifier:HKWorkoutRouteTypeIdentifier],
	*/
																	nil
			];

			@try {
				[_store
						requestAuthorizationToShareTypes: identifier_set
						readTypes: identifier_set
						completion: completion_block
				];
			}
			@catch (NSException* exception) {
				qCritical() << "Failed during request HealthKit authorization: " << QString::fromNSString([exception reason]);
				return false;
			}
			@finally {
	//			[identifier_set dealloc];
			}

			return true;
		}

	public:
		virtual bool requestAccess() override
		{
			qDebug() << "Requesting access to HealthKit data...";

			return request(
							^(BOOL success, NSError* error)
							{
								dispatch_sync(
												dispatch_get_main_queue(),
												^{
													if (NO == success || error) {
														qCritical() << "Failed during request HealthKit authorization: " << QString::fromNSString(error.localizedDescription);
													} else {
														emit registered();
													}
												}
								);
							}
			);
		}

		virtual bool revokeAccess() override
		{
			return false;
		}

		virtual FitnessTracker::Family getFamily() const override
		{
			return FitnessTracker::Family::Apple;
		}

		bool isAvaliable() const
		{
			 return [HKHealthStore isHealthDataAvailable] == YES;
		}

		bool hasStore()
		{
			return isAvaliable() && _store;
		}

		virtual bool hasAccess() const override
		{
			return isAvaliable();
		}
	};

	FitnessTracker* FitnessTracker::Instance()
	{
		static HealthKit retval;
		return &retval;
	}
} } } }



