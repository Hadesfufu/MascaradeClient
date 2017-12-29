
#include "FontHolder.h"

FontHolder::FontHolder():
m_Font_Path("data/fonts/")
{

}
FontHolder::~FontHolder()
{
	m_Fonts.clear();
}

sf::Font* FontHolder::getFont(const std::string& asName){
	// if the texture is already hold...
	auto savedFont = m_Fonts.find(asName);
	if (savedFont != m_Fonts.end()){
		// ... we return it.
		return savedFont->second.get();
	}
	
	// otherwise we load it
	sf::Font* font = new sf::Font();
	if (font->loadFromFile(m_Font_Path + asName))
	{
		// load success : we hold it and return a pointer
		auto elem = m_Fonts.emplace(asName, std::unique_ptr<sf::Font>(font));
		Log::info("Font Chargée") << " ! path : " + m_Font_Path + asName;
		return elem.first->second.get();
//		return m_Fonts.at(asName).get();
	}
	else
	{
		// load failed : we print a Error message and return a null pointer.
		delete font;
		Log::error("FontHolder") << "Impossible de trouver la font : path : " + m_Font_Path + asName;
		return nullptr;
	}
}