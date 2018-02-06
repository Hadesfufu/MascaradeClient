#include "LoadingScreen.h"

LoadingScreen::LoadingScreen():
m_Continue(true),
m_LoadingThread(&LoadingScreen::t_func, this)
{
	m_Text.setFont(*FontHolder::I()->getFont("8bit.ttf"));
	m_Text.setString("Loading");
	m_Text.setCharacterSize(60);
	m_Text.setOrigin(m_Text.getGlobalBounds().width / 2, m_Text.getGlobalBounds().height / 2);
	m_Text.setFillColor(sf::Color::White);
	sf::RenderWindow* window = Application::I()->getWindow();
	if (window && window->isOpen()){
		m_Text.setPosition((float)(window->getSize().x / 2), (float)(window->getSize().y /  2.5f));
	}
}


LoadingScreen::~LoadingScreen()
{

}

void LoadingScreen::Launch(){
	m_Continue = true;
	Application::I()->getWindow()->setActive(false);
	m_LoadingThread.launch();
}

void LoadingScreen::Stop(){
	m_Mutex.lock();
	m_Continue = false;
	m_Mutex.unlock();
	m_LoadingThread.wait();
	Application::I()->getWindow()->setActive(true);
}

void LoadingScreen::t_func(){
	sf::RenderWindow *window = Application::I()->getWindow();
	window->setActive(true);
	while (window && window->isOpen() && m_Continue){
		m_Mutex.unlock();
		window->clear(sf::Color::Black);
		window->draw(m_Text);
		window->display();
		m_Mutex.lock();
	}
	window->setActive(false);
}

void LoadingScreen::setIsCurrentController(bool b){

}