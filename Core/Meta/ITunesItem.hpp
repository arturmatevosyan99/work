#ifndef Cadabra_Queen_Core_Net_Meta_ITunesItem_hpp
#define Cadabra_Queen_Core_Net_Meta_ITunesItem_hpp

#include <QObject>
#include <QJsonObject>
#include <QString>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class ITunesItem
	{
	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_PROPERTY(QString id READ getId)
		Q_PROPERTY(QString censoredName READ getCensoredName)
		Q_PROPERTY(QString trackName READ getTrackName)
		Q_PROPERTY(QString description READ getDescription)
		Q_PROPERTY(QString version READ getVersion)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ITunesItem();
		ITunesItem(int id, const QString& ident, const QString& censored_name, const QString& track_name, const QString& description, const QString& version);
		ITunesItem(const ITunesItem& origin);
		ITunesItem(ITunesItem&& origin);
		~ITunesItem();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		int _id;
		QString _ident;
		QString _censored_name;
		QString _track_name;
		QString _description;
		QString _version;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		ITunesItem& operator=(const ITunesItem& rhs);
		ITunesItem& operator=(ITunesItem&& rhs);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const int& getId() const;
		const QString& getIdent() const;
		const QString& getCensoredName() const;
		const QString& getTrackName() const;
		const QString& getDescription() const;
		const QString& getVersion() const;
	/** @} */


	/** @name States */
	/** @{ */
	public:
		bool isZeroVersion() const;
		bool isHigher(const ITunesItem& rhs) const;
		bool isHigher(const QString& rhs) const;
	/** @} */
	};
} } } }

/*
{
  "screenshotUrls": [
	"https://is2-ssl.mzstatic.com/image/thumb/Purple123/v4/df/85/0a/df850a7a-2ebb-a02f-bea6-767208b646b7/pr_source.png/392x696bb.png",
	"https://is1-ssl.mzstatic.com/image/thumb/Purple123/v4/7e/8e/17/7e8e1752-a3a9-2e69-7023-456f8cdc154d/pr_source.png/392x696bb.png",
	"https://is4-ssl.mzstatic.com/image/thumb/Purple123/v4/c0/92/0c/c0920c8f-0e09-9a3a-5654-b7e9c537e2e6/pr_source.png/392x696bb.png",
	"https://is1-ssl.mzstatic.com/image/thumb/Purple123/v4/61/8a/32/618a327a-1a86-4199-3fed-8e27caa0a0a8/pr_source.png/392x696bb.png",
	"https://is3-ssl.mzstatic.com/image/thumb/Purple123/v4/4a/99/e0/4a99e0d4-2859-2c00-70e7-fcde1b33a537/pr_source.png/392x696bb.png"
  ],
  "ipadScreenshotUrls": [],
  "appletvScreenshotUrls": [],
  "artworkUrl60": "https://is4-ssl.mzstatic.com/image/thumb/Purple125/v4/2a/0f/a8/2a0fa85c-dc55-014c-7edc-aeca2164c4c1/source/60x60bb.jpg",
  "artworkUrl512": "https://is4-ssl.mzstatic.com/image/thumb/Purple125/v4/2a/0f/a8/2a0fa85c-dc55-014c-7edc-aeca2164c4c1/source/512x512bb.jpg",
  "artworkUrl100": "https://is4-ssl.mzstatic.com/image/thumb/Purple125/v4/2a/0f/a8/2a0fa85c-dc55-014c-7edc-aeca2164c4c1/source/100x100bb.jpg",
  "artistViewUrl": "https://apps.apple.com/us/developer/interfacecontrol/id405950758?uo=4",
  "supportedDevices": [
	"iPhone5s-iPhone5s",
	"iPadAir-iPadAir",
	"iPadAirCellular-iPadAirCellular",
	"iPadMiniRetina-iPadMiniRetina",
	"iPadMiniRetinaCellular-iPadMiniRetinaCellular",
	"iPhone6-iPhone6",
	"iPhone6Plus-iPhone6Plus",
	"iPadAir2-iPadAir2",
	"iPadAir2Cellular-iPadAir2Cellular",
	"iPadMini3-iPadMini3",
	"iPadMini3Cellular-iPadMini3Cellular",
	"iPodTouchSixthGen-iPodTouchSixthGen",
	"iPhone6s-iPhone6s",
	"iPhone6sPlus-iPhone6sPlus",
	"iPadMini4-iPadMini4",
	"iPadMini4Cellular-iPadMini4Cellular",
	"iPadPro-iPadPro",
	"iPadProCellular-iPadProCellular",
	"iPadPro97-iPadPro97",
	"iPadPro97Cellular-iPadPro97Cellular",
	"iPhoneSE-iPhoneSE",
	"iPhone7-iPhone7",
	"iPhone7Plus-iPhone7Plus",
	"iPad611-iPad611",
	"iPad612-iPad612",
	"iPad71-iPad71",
	"iPad72-iPad72",
	"iPad73-iPad73",
	"iPad74-iPad74",
	"iPhone8-iPhone8",
	"iPhone8Plus-iPhone8Plus",
	"iPhoneX-iPhoneX",
	"iPad75-iPad75",
	"iPad76-iPad76",
	"iPhoneXS-iPhoneXS",
	"iPhoneXSMax-iPhoneXSMax",
	"iPhoneXR-iPhoneXR",
	"iPad812-iPad812",
	"iPad834-iPad834",
	"iPad856-iPad856",
	"iPad878-iPad878",
	"iPadMini5-iPadMini5",
	"iPadMini5Cellular-iPadMini5Cellular",
	"iPadAir3-iPadAir3",
	"iPadAir3Cellular-iPadAir3Cellular",
	"iPodTouchSeventhGen-iPodTouchSeventhGen",
	"iPhone11-iPhone11",
	"iPhone11Pro-iPhone11Pro",
	"iPadSeventhGen-iPadSeventhGen",
	"iPadSeventhGenCellular-iPadSeventhGenCellular",
	"iPhone11ProMax-iPhone11ProMax",
	"iPhoneSESecondGen-iPhoneSESecondGen",
	"iPadProSecondGen-iPadProSecondGen",
	"iPadProSecondGenCellular-iPadProSecondGenCellular",
	"iPadProFourthGen-iPadProFourthGen",
	"iPadProFourthGenCellular-iPadProFourthGenCellular",
	"iPhone12Mini-iPhone12Mini",
	"iPhone12-iPhone12",
	"iPhone12Pro-iPhone12Pro",
	"iPhone12ProMax-iPhone12ProMax",
	"iPadAir4-iPadAir4",
	"iPadAir4Cellular-iPadAir4Cellular",
	"iPadEighthGen-iPadEighthGen",
	"iPadEighthGenCellular-iPadEighthGenCellular",
	"iPadProThirdGen-iPadProThirdGen",
	"iPadProThirdGenCellular-iPadProThirdGenCellular",
	"iPadProFifthGen-iPadProFifthGen",
	"iPadProFifthGenCellular-iPadProFifthGenCellular"
  ],
  "advisories": [
	"Frequent/Intense Medical/Treatment Information"
  ],
  "isGameCenterEnabled": false,
  "features": [],
  "kind": "software",
  "minimumOsVersion": "12.0",
  "trackCensoredName": "Худеем Онлайн — Сергей Обложко",
  "languageCodesISO2A": [],
  "fileSizeBytes": "40547328",
  "sellerUrl": "https://smart.hudeem-online.ru",
  "formattedPrice": "Free",
  "contentAdvisoryRating": "17+",
  "averageUserRatingForCurrentVersion": 5,
  "userRatingCountForCurrentVersion": 9,
  "trackViewUrl": "https://apps.apple.com/us/app/%D1%85%D1%83%D0%B4%D0%B5%D0%B5%D0%BC-%D0%BE%D0%BD%D0%BB%D0%B0%D0%B9%D0%BD-%D1%81%D0%B5%D1%80%D0%B3%D0%B5%D0%B9-%D0%BE%D0%B1%D0%BB%D0%BE%D0%B6%D0%BA%D0%BE/id1475097120?uo=4",
  "trackContentRating": "17+",
  "averageUserRating": 5,
  "genreIds": [
	"6013",
	"6020"
  ],
  "trackId": 1475097120,
  "trackName": "Худеем Онлайн — Сергей Обложко",
  "releaseDate": "2020-01-10T08:00:00Z",
  "sellerName": "Sergey Kudryashov",
  "primaryGenreName": "Health & Fitness",
  "isVppDeviceBasedLicensingEnabled": true,
  "currentVersionReleaseDate": "2021-06-10T23:15:01Z",
  "releaseNotes": "| Исправили ошибку подключения на старте приложения",
  "primaryGenreId": 6013,
  "currency": "USD",
  "description": "Авторская методика \n\n ... .",
  "artistId": 405950758,
  "artistName": "InterfaceControl",
  "genres": [
	"Health & Fitness",
	"Medical"
  ],
  "price": 0,
  "bundleId": "systems.cadabra.dev.queen-afrodita",
  "version": "1.1.82",
  "wrapperType": "software",
  "userRatingCount": 9
}
*/

Q_DECLARE_METATYPE(Cadabra::Queen::Core::Meta::ITunesItem)

#endif
