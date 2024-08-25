#ifndef MOSCBRIDGE_H
#define MOSCBRIDGE_H
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "msc.h"
#include "helper/Path.h"
#define MAX_METHODS_PER_CLASS 50
#define MAX_CLASSES_PER_MODULE 50
#define MAX_MODULES_PER_LIBRARY 50

#pragma once

namespace ntolen
{
    class MethodRegistry
    {
    public:
        MethodRegistry(): _signature(""), _method(nullptr) {}
        MethodRegistry(const char *signature, bool isStatic, MSCExternMethodFn method);
        bool isStatic() const { return _isStatic; }
        const std::string &signature() const { return _signature; }
        MSCExternMethodFn method() const { return _method; }
        bool isValid() {
            return _method == nullptr;
        }
    private:
        bool _isStatic;
        std::string _signature;
        MSCExternMethodFn _method;
    };
    class ClassRegistry
    {
    public:
        ClassRegistry(const char *name);
        MethodRegistry method(const char *signature, bool isStatic);
        void newMethod(const char *signature, bool isStatic, MSCExternMethodFn value);
        const std::string &name() const { return _name; }
        const std::map<std::string, MethodRegistry> &methods() const { return _methods; }

    private:
        std::string _name;
        std::map<std::string, MethodRegistry> _methods;
    };
    class ModuleRegistry
    {
    public:
        ModuleRegistry():_source(nullptr) {}
        ~ModuleRegistry() {}
        ModuleRegistry(const char *name, const char **source);
        ClassRegistry *clazz(const char *clazz);
        void newClass(ClassRegistry &clazz);
        const std::string name() const
        {
            return _name;
        }
        bool isValid() const { 
            return _source != nullptr;
         }
        const char** & source()
        {
            return _source;
        }
        const std::map<std::string, ClassRegistry> &classes() const
        {
            return _classes;
        }

    private:
        std::string _name;

        const char** _source;

        std::map<std::string, ClassRegistry> _classes;
    };

    class PackageRegistry
    {
    public:
        PackageRegistry(const char *name);
        ModuleRegistry module(const char *name);
        void newModule(ModuleRegistry &module);

        const std::string &name() const { return _name; }
        std::map<std::string, ModuleRegistry> modules() { return _modules; }

    private:
        std::string _name;
        std::map<std::string, ModuleRegistry> _modules;
    };
    class MoscRuntime
    {
    public:
        MoscRuntime(MSCConfig *config);
        ~MoscRuntime();
        static void install(PackageRegistry package);
        MVM *vm() const { return _vm; }
        MSCBindExternClassFn bindClassFn;
        MSCBindExternMethodFn bindMethodFn;
        InterpretResult run(const char *root, std::string module, std::string source);
        InterpretResult runFile(const char *file);
        void setExitCode(int code)
        {
            _exitCode = code;
        }
        const int exitCode() const
        {
            return _exitCode;
        }

        static std::vector<PackageRegistry> packages;
        static ModuleRegistry module(const char *name);
        MSCHandle *makeHandle(Djuru *djuru, int slot, const char *module, const char *name);
        MSCHandle *makeCallHandle(const char *signature);
    private:
        MSCExternClassMethods bindBuiltInExternClass(const char *moduleName, const char *className);
        MSCExternMethodFn bindBuiltInExternMethod(const char *module,
                                                  const char *className, bool isStatic, const char *signature);
        MSCLoadModuleResult loadBuiltInModule(const char *name);
        std::string resolveModule(const char *importer, const char *name);
        const char* moduleSource(const char *name);
        struct MVM *_vm;
        MSCConfig *_config;
        int _exitCode;
        const char *_rootDirectory;
        std::unique_ptr<Path> _moduleDirectory;
        std::map<std::string, MSCHandle *> handles;

        void findModulesDirectory();
    };
    class ModuleBuilder;
    class PackageBuilder;
    class ClassBuilder;
    class ClassProvider {
        public:
        virtual ~ClassProvider() {};
        virtual void provide(ClassBuilder& builder) = 0;
        virtual const char* className() const = 0;
    };
    class ModuleProvider {
        public:
        virtual ~ModuleProvider() {};
        virtual void provide(ModuleBuilder& builder) = 0;
        virtual const char* moduleName() const = 0;
        virtual const char** moduleSource() const = 0;
    };
    class PackageBuilder
    {
    public:
        PackageBuilder(const char *name): registry(name)
        {
        }
        ModuleBuilder module(const char *name, const char **source);
        PackageBuilder& provide(ModuleProvider& provider);
        PackageRegistry build()
        {
            return registry;
        }
        void collect(ModuleRegistry &moduleRegistry)
        {
            registry.newModule(moduleRegistry);
        }

    private:
        PackageRegistry registry;
    };
    class ModuleBuilder
    {
    public:
        ModuleBuilder(PackageBuilder& p, const char *name, const char **source) : parent(p), registry(name, source)
        {
        }
        ClassBuilder clazz(const char *name);
        ModuleBuilder& provide(ClassProvider& provider);
        ModuleRegistry& build()
        {
            return registry;
        }
        void collect(ClassRegistry &clazz)
        {
            registry.newClass(clazz);
            
        }
        PackageBuilder &end()
        {
            parent.collect(build());
            return parent;
        }

    private:
        PackageBuilder &parent;
        ModuleRegistry registry;
    };
    class ClassBuilder
    {
    public:
        ClassBuilder(ModuleBuilder &p, const char *name) : parent(p), registry(name)
        {
        }

        ClassBuilder &constructor(MSCExternMethodFn fn)
        {
            registry.newMethod("<allocate>", true, fn);
            return *this;
        }
        ClassBuilder &finalizer(MSCFinalizerFn fn)
        {
            registry.newMethod("<finalize>", true, (MSCExternMethodFn)fn);
            return *this;
        }
        ClassBuilder &method(bool isStatic, const char *signature, MSCExternMethodFn fn)
        {
            registry.newMethod(signature, isStatic, fn);
            return *this;
        }
        ClassRegistry &build()
        {
            return registry;
        }
        ModuleBuilder &end()
        {
            parent.collect(build());
            return parent;
        }

    private:
        ModuleBuilder &parent;
        ClassRegistry registry;
    };
};

#endif