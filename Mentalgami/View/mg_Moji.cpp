#include "mg_Moji.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace View {
	Moji::Moji(QObject* parent)
	:
		QObject(parent)
	{

	}

	Moji::~Moji()
	{

	}

	bool Moji::load(const QString& family, const QString& name)
	{
		const int dot_offset(name.lastIndexOf("."));
		if (dot_offset < 0) {
			emit loaded(false);
			return false;
		}
		_family = family;
		_collection = name.left(dot_offset);
		_symbol = name.right(name.length() - dot_offset - 1);

		Mentalgami::Net::MojiFile file(family);
		if (file.load(_collection) != Net::MojiFile::State::Cached) {
			emit loaded(false);
			return false;
		}
		const Mentalgami::Net::MojiFile::E e(file.locate(_symbol));
		_url = e.url;
		_unicode = e.unicode;
		_meaning = e.meaning;
		emit loaded(true);
		return true;
	}

	const QUrl& Moji::getURL() const
	{
		return _url;
	}

	const QChar& Moji::getUnicode() const
	{
		return _unicode;
	}

	const QString& Moji::getMeaning() const
	{
		return _meaning;
	}

	void Moji::set(const QUrl& url, const QString& meaning, const QChar& unicode)
	{
		if (_url != url || _meaning != meaning || _unicode != unicode) {
			_url = url;
			_meaning = meaning;
			_unicode = unicode;
			emit loaded(true);
		}
	}

	void Moji::setURL(const QUrl& value)
	{
		if (_url != value) {
			_url = value;
			emit loaded(true);
		}
	}

	void Moji::setUnicode(const QChar& value)
	{
		if (_unicode != value) {
			_unicode = value;
			emit loaded(true);
		}
	}

	void Moji::setMeaning(const QString& value)
	{
		if (_meaning != value) {
			_meaning = value;
			emit loaded(true);
		}
	}
} } } }
