///////////////////////////////////////////////
//	created by Vassili "Bakasama" Rezvoy
//	02/04/2016
//	project zeraventure
///////////////////////////////////////////////

#ifndef NOTIFICATION_MANAGER_H_INCLUDED
#define NOTIFICATION_MANAGER_H_INCLUDED

#include <list>
#include <functional>
#include <string>
#include <map>
#include "Singleton.h"

class NotificationManager : public Singleton<NotificationManager>{
	friend class Singleton<NotificationManager>;
public:
	///////////////////////////////////////////////
	//	struct containing a std::map<std::string, void *>
	//	reason : avoid warning
	//	usage : use it to share datas beetwen object
	//	beware key object is reserved
	//	you have to cast void* elements to use it
	///////////////////////////////////////////////
	struct Dictionary
	{
		std::string						m_notificationName;
		void *							m_object;
		std::map<std::string, void *>	m_dict;
	};

	///////////////////////////////////////////////
	//	AddObserver
	//	@name : nom de la notification, utilisé pour lancer la notification
	//	@observed : objet qui s'enregistre à une notification
	//	@function : fonction qui sera appellé si la notification est lancée
	///////////////////////////////////////////////
	template <class T>
	void AddObserver(std::string name, T *observer, void (T::*function)(void)){
		if (observer == nullptr)
			return;

		auto binded = std::bind(function, observer);
		auto callback = std::function<void()>(binded);

		BasicObjectStorage storage;
		storage.m_typeId = typeid(T).hash_code();
		storage.m_object = observer;
		storage.m_function = callback;

		m_datasBasic[name].push_back(storage);
	}
	template <class T>
	void AddObserver(std::string name, T *observer, void (T::*function)(Dictionary)){
		if (observer == nullptr)
			return;

		auto binded = std::bind(function, observer, std::placeholders::_1);
		auto callback = std::function<void(Dictionary)>(binded);

		AdvancedObjectStorage storage;
		storage.m_typeId = typeid(T).hash_code();
		storage.m_object = observer;
		storage.m_function = callback;

		m_datasAdvanced[name].push_back(storage);
	}

	///////////////////////////////////////////////
	//	RemoveObserver
	//	@observer : objet qui se désinscrit des notifications
	//	@name : nom de la notification dont on doit désinscrire l'objet
	//	si aucun @name n'est passé l'objet est retiré de toute les notifications
	///////////////////////////////////////////////
	template <class T>
	void RemoveObserver(T *observer){
		for (auto it = m_datasBasic.begin(); it != m_datasBasic.end(); ){
			std::list<BasicObjectStorage> &list = it->second;
			for (auto it2 = list.begin(); it2 != list.end(); ){
				if (typeid(T).hash_code() == (*it2).m_typeId){
					T *elem = static_cast<T *>((*it2).m_object);
					if (elem == observer)
						it2 = list.erase(it2);
					else
						++it2;
				}
				else
					++it2;
			}

			if (list.size() == 0)
				it = m_datasBasic.erase(it);
			else
				it++;
		}

		for (auto it = m_datasAdvanced.begin(); it != m_datasAdvanced.end(); ){
			std::list<AdvancedObjectStorage> &list = it->second;
			for (auto it2 = list.begin(); it2 != list.end(); ){
				if (typeid(T).hash_code() == (*it2).m_typeId){
					T *elem = static_cast<T *>((*it2).m_object);
					if (elem == observer)
						it2 = list.erase(it2);
					else
						++it2;
				}
				else
					++it2;
			}

			if (list.size() == 0)
				it = m_datasAdvanced.erase(it);
			else
				it++;
		}
	}
	template <class T>
	void RemoveObserver(T *observer, std::string name){
		auto basicListIt = m_datasBasic.find(name);
		if (basicListIt != m_datasBasic.end()){
			std::list<BasicObjectStorage> &basicList = basicListIt->second;
			for (auto it = basicList.begin(); it != basicList.end();){
				if (typeid(T).hash_code() == (*it).m_typeId){
					T *elem = static_cast<T *>((*it).m_object);
					if (elem == observer){
						it = basicList.erase(it);
					}
					else{
						++it;
					}
				}
				else{
					++it;
				}
			}
		}

		auto advanceListIt = m_datasAdvanced.find(name);
		if (advanceListIt != m_datasAdvanced.end()){
			std::list<AdvancedObjectStorage> &advanceList = advanceListIt->second;
			for (auto it = advanceList.begin(); it != advanceList.end();){
				if (typeid(T).hash_code() == (*it).m_typeId){
					T *elem = static_cast<T *>((*it).m_object);
					if (elem == observer){
						it = advanceList.erase(it);
					}
					else{
						++it;
					}
				}
				else{
					++it;
				}
			}
		}
	}

	///////////////////////////////////////////////
	//	PostNotification
	//	@name : nom de la notification à lancer
	//	@object : objet à partager à l'objet qui à souscrit à la notification, il sera récupérable dans le dictionaire sous la clef object
	//	@userInfo : données à partager à l'objet qui à souscrit à la notification
	///////////////////////////////////////////////
	void PostNotification(std::string name){
		auto itL = m_datasBasic.find(name);
		if (itL != m_datasBasic.end()){
			std::list<BasicObjectStorage> list = itL->second;
			for (auto it = list.begin(); it != list.end(); it++)
				(*it).m_function();
		}

		auto itL2 = m_datasAdvanced.find(name);
		if (itL2 != m_datasAdvanced.end()){
			Dictionary userInfo;
			userInfo.m_notificationName = std::string(name);
			userInfo.m_object = nullptr;

			std::list<AdvancedObjectStorage> list = itL2->second;
			for (auto it = list.begin(); it != list.end(); it++)
				(*it).m_function(userInfo);
		}
	}
	template <class T>
	void PostNotification(std::string name, T *object){
		auto itL = m_datasBasic.find(name);
		if (itL != m_datasBasic.end()){
			std::list<BasicObjectStorage> list = itL->second;
			for (auto it = list.begin(); it != list.end(); it++)
				(*it).m_function();
		}

		auto itL2 = m_datasAdvanced.find(name);
		if (itL2 != m_datasAdvanced.end()){
			Dictionary userInfo;
			userInfo.m_notificationName = std::string(name);
			userInfo.m_object = object;

			std::list<AdvancedObjectStorage> list = itL2->second;
			for (auto it = list.begin(); it != list.end(); it++)
				(*it).m_function(userInfo);
		}
	}
	template <class T>
	void PostNotification(std::string name, T *object, Dictionary userInfo){
		auto itL = m_datasBasic.find(name);
		if (itL != m_datasBasic.end()){
			std::list<BasicObjectStorage> list = itL->second;
			for (auto it = list.begin(); it != list.end(); it++){
				(*it).m_function();
			}
		}

		auto itL2 = m_datasAdvanced.find(name);
		if (itL2 != m_datasAdvanced.end()){
			userInfo.m_notificationName = std::string(name);
			userInfo.m_object = object;

			std::list<AdvancedObjectStorage> list = itL2->second;
			for (auto it = list.begin(); it != list.end(); it++)
				(*it).m_function(userInfo);
		}
	}
	void PostNotification(std::string name, Dictionary userInfo){
		auto itL = m_datasBasic.find(name);
		if (itL != m_datasBasic.end()){
			std::list<BasicObjectStorage> list = itL->second;
			for (auto it = list.begin(); it != list.end(); it++)
				(*it).m_function();
		}

		auto itL2 = m_datasAdvanced.find(name);
		if (itL2 != m_datasAdvanced.end()){
			userInfo.m_notificationName = std::string(name);
			userInfo.m_object = nullptr;

			std::list<AdvancedObjectStorage> list = itL2->second;
			for (auto it = list.begin(); it != list.end(); it++)
				(*it).m_function(userInfo);
		}
	}

private:
	NotificationManager();
	~NotificationManager();

private:
	struct BasicObjectStorage
	{
		size_t												m_typeId;
		void *												m_object;
		std::function<void()>								m_function;
	};
	struct AdvancedObjectStorage
	{
		size_t												m_typeId;
		void *												m_object;
		std::function<void(Dictionary)>						m_function;
	};

private:
	std::map<std::string, std::list<BasicObjectStorage>>	m_datasBasic;
	std::map<std::string, std::list<AdvancedObjectStorage>>	m_datasAdvanced;
};

#endif