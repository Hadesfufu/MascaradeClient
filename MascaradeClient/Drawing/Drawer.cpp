
#include "Drawer.h"
#include <iostream>

Drawer::Drawer() : 
m_Window(nullptr)
{
	loadStaticLayers();
}
Drawer::~Drawer()
{
	clear();
}

void Drawer::draw(){
	if (!m_Window->isOpen())
		return;
	updateAnims();
	//updateParallax();
	m_Window->clear(sf::Color(208, 244, 247));
	m_Window->setView(m_Camera);
	for (auto it = m_Layers.begin(); it != m_Layers.end() && !it->second.hidden; ++it){
		for (auto it2 = it->second.drawables.begin(); it2 != it->second.drawables.end(); ++it2){
			if (!it2->drawable)
			{
				Log::error("Drawer - draw") << "Null exception" << "The entry will be removed";
				it2 = it->second.drawables.erase(it2);
				continue;
			}
			if (!it2->hidden){
				if (it2->shader){
					m_Window->draw(*it2->drawable, it2->shader);
				}
				else{
					m_Window->draw(*it2->drawable);
				}
			}
		}
    }
	m_Window->display();
}

void Drawer::updateAnims(){
	DrawingAnimation* deletor;
	for (auto it = m_Animations.begin(); !m_Animations.empty() && it != m_Animations.end(); ++it){
		if ((*it)->isAlive())
			(*it)->update();
		else{
			deletor = it->release();
			it = m_Animations.erase(it);
			delete deletor;
			deletor = nullptr;
			break;
		}
	}
}

void Drawer::addToLayer(int layer, const sf::Drawable* drawable, sf::Shader* shader){

	if (m_Layers.find(layer) == m_Layers.end())
		m_Layers[layer] =  DrawerStructures::Z_index();
	m_Layers[layer].drawables.push_back(DrawerStructures::Drawable(drawable, shader));
}

void Drawer::addToLayer(std::string layer, const sf::Drawable* drawable, sf::Shader* shader)
{
	if (m_StaticLayers.find(layer) != m_StaticLayers.end())
		addToLayer(m_StaticLayers[layer], drawable, shader);
	else
 	Log::warning("Drawer - addToLayer") << "Specified static layer not found";
}

void Drawer::hideLayer(int layer){
	m_Layers[layer].hidden = true;	
}

void Drawer::hideLayer(std::string layer)
{
	if (m_StaticLayers.find(layer) != m_StaticLayers.end())
		hideLayer(m_StaticLayers[layer]);
	else
		Log::warning("Drawer - HideLayer") << "Specified static layer not found";
}


void Drawer::hideElement(const sf::Drawable* elem){
	for (auto it = m_Layers.begin(); it != m_Layers.end(); ++it){
		DrawerStructures::Z_index& zIndex = it->second;
		for (auto it2 = zIndex.drawables.begin(); it2 != zIndex.drawables.end(); ++it2){
			if (it2->drawable == elem){
				DrawerStructures::Drawable& drawable = *it2;
				drawable.hidden = true;
			}
		}
	}
}

void Drawer::showLayer(int layer){
	m_Layers[layer].hidden = false;	
}

void Drawer::showLayer(std::string layer)
{
	if (m_StaticLayers.find(layer) != m_StaticLayers.end())
		showLayer(m_StaticLayers[layer]);
	else
		Log::warning("Drawer - ShowLayer") << "Specified static layer not found";
}

void Drawer::showElement(const sf::Drawable* elem){
	for (auto it = m_Layers.begin(); it != m_Layers.end(); ++it){
		for (auto it2 = it->second.drawables.begin(); it2 != it->second.drawables.end(); ++it2){
			if (it2->drawable == elem){
				DrawerStructures::Drawable& drawable = *it2;
				drawable.hidden = false;
				//Log::debug() << "Element Showing successfull";
			}
		}
	}
}

void Drawer::clear(){
	m_Layers.clear();
}

void Drawer::removeLayer(int layer){
	
	if (m_Layers.find(layer) != m_Layers.end())
		m_Layers.erase(layer);
	
}

void Drawer::removeLayer(std::string layer)
{
	if (m_StaticLayers.find(layer) != m_StaticLayers.end())
		removeLayer(m_StaticLayers[layer]);
	else
		Log::warning("Drawer - HideLayer") << "Specified static layer not found";
}

DrawerStructures::Z_index* Drawer::getLayer(int i){
	if (m_Layers.find(i) != m_Layers.end())
		return &m_Layers[i];
	else return nullptr;
}

DrawerStructures::Z_index* Drawer::getLayer(std::string layer)
{
	if (m_StaticLayers.find(layer) != m_StaticLayers.end())
		return getLayer(m_StaticLayers[layer]);
	else
		Log::warning("Drawer - HideLayer") << "Specified static layer not found";
	return nullptr;
}

bool Drawer::removeElement(sf::Drawable* elem){
	for (auto it = m_Layers.begin(); it != m_Layers.end(); ++it){
		for (auto it2 = it->second.drawables.begin(); it2 != it->second.drawables.end(); ++it2){
			if (it2->drawable == elem){
				it->second.drawables.erase(it2);
				return true;
			}
		}
	}

	return false;
}

DrawerStructures::Drawable* Drawer::getDrawable(const sf::Drawable* elem){
	for (auto it = m_Layers.begin(); it != m_Layers.end(); ++it){
		DrawerStructures::Z_index zIndex = it->second;
		for (auto it2 = zIndex.drawables.begin(); it2 != zIndex.drawables.end(); ++it2){
			if (it2->drawable == elem){
				 return &(*it2);
			}
		}
	}
	return nullptr;
}

void Drawer::addAnimation(DrawingAnimation* anim){
	m_Animations.emplace_back(anim);
}

sf::Time Drawer::interruptAnimation(DrawingAnimation* anim)
{
	sf::Time returner = sf::seconds(-1);
	for(auto it = m_Animations.begin(); it != m_Animations.end(); ++it)
	{
		if (anim == it->get()) {
			returner = it->get()->getDuration();
			DrawingAnimation* a = it->release();
			delete a;
			it = m_Animations.erase(it);
			return returner;
		}
	}
	return returner;
}

bool Drawer::setWindow(sf::RenderWindow* window){
	m_Window = window;
	sf::Vector2u size = m_Window->getSize();
	m_Camera.setSize(sf::Vector2f((float)size.x, (float)size.y));
	m_Camera.setCenter((float)size.x / 2, (float)size.y / 2);
	return true; 
}

void Drawer::loadStaticLayers()
{
	if (Data::exist("StaticLayers")) {
		for (auto& layer : Data::json().at("StaticLayers")) {
			m_StaticLayers.emplace(layer.at("name"), layer.at("id"));
		}
	}
}
