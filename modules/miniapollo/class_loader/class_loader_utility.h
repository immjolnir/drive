#pragma once

// RegisterClass 由 class_loader_register_macro.h 来调用的 template <typename Derived, typename Base>
void RegisterClass(const std::string& class_name, const std::string& base_class_name) {
    AINFO << "registerclass:" << class_name << "," << base_class_name << "," << GetCurLoadingLibraryName();

    utility::AbstractClassFactory<Base>* new_class_factory_obj =
      new utility::ClassFactory<Derived, Base>(class_name, base_class_name);  // 使用raw pointer
    new_class_factory_obj->AddOwnedClassLoader(GetCurActiveClassLoader());
    new_class_factory_obj->SetRelativeLibraryPath(GetCurLoadingLibraryName());

    GetClassFactoryMapMapMutex().lock();
    ClassClassFactoryMap& factory_map = GetClassFactoryMapByBaseClass(typeid(Base).name());
    factory_map[class_name] = new_class_factory_obj;
    GetClassFactoryMapMapMutex().unlock();
}

template <typename Base>
Base* CreateClassObj(const std::string& class_name, ClassLoader* loader) {
    GetClassFactoryMapMapMutex().lock();
    ClassClassFactoryMap& factoryMap =
      GetClassFactoryMapByBaseClass(typeid(Base).name());  // 这应该依赖于 RegisterCalss。调用顺序
    AbstractClassFactory<Base>* factory = nullptr;
    if (factoryMap.find(class_name) != factoryMap.end()) {
        factory = dynamic_cast<utility::AbstractClassFactory<Base>*>(factoryMap[class_name]);
    }
    GetClassFactoryMapMapMutex().unlock();

    Base* classobj = nullptr;
    if (factory && factory->IsOwnedBy(loader)) {
        classobj = factory->CreateObj();
    }

    return classobj;
}
