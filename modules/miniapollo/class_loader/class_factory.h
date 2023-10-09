#pragma once

#include <string>
#include <vector>

namespace class_loader
{
    class ClassLoader;

    namespace utility
    {
        class AbstractClassFactoryBase {
          public:
            AbstractClassFactoryBase(const std::string& class_name, const std::string& base_class_name);
            virtual ~AbstractClassFactoryBase();

            void SetRelativeLibraryPath(const std::string& library_path);
            void AddOwnedClassLoader(ClassLoader* loader);
            void RemoveOwnedClassLoader(const ClassLoader* loader);
            bool IsOwnedBy(const ClassLoader* loader);
            bool IsOwnedByAnybody();
            std::vector<ClassLoader*> GetRelativeClassLoaders();
            const std::string GetRelativeLibraryPath() const;
            const std::string GetBaseClassName() const;
            const std::string GetClassName() const;

          protected:
            std::vector<ClassLoader*> relative_class_loaders_;
            std::string relative_library_path_;
            std::string base_class_name_;
            std::string class_name_;
        };

        template <typename ClassObject, typename Base>
        class ClassFactory : public AbstractClassFactoryBase {
          public:
            ClassFactory(const std::string& class_name, const std::string& base_class_name)
              : AbstractClassFactoryBase(class_name, base_class_name) {}

            // ClassObject是 derived class， 模板创建时，给定的。直到该方法被调用，才真正创建对象. 这是神奇的地方
            Base* CreateObj() const { return new ClassObject; }
        };
    }  // namespace utility
}  // namespace class_loader
