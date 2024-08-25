#include "Mosc.h"
#include "msc.h"
#include <iostream>
#include "helper/ResourceManager.h"

using namespace ntolen;
struct MVM;

/// @brief Constrcut new MethodRegistry
MethodRegistry::MethodRegistry(const char *signature, bool isStatic, MSCExternMethodFn method) : _signature(signature), _isStatic(isStatic), _method(method) {}

ClassRegistry::ClassRegistry(const char *name) : _name(name), _methods() {}

MethodRegistry ClassRegistry::method(const char *signature, bool isStatic)
{
    std::string key = (isStatic ? "static " : "") + std::string(signature);
    auto met = _methods.find(key);
    if (met != _methods.end())
    {
        std::cout << "Fetching method " << key << ':' << met->second.method() << std::endl;
        return _methods.at(key);
    }
    else
    {
        for (auto const &[key, val] : _methods)
        {
            std::cout << "Key:" << key << (val.isStatic() ? "static " : "") << signature << ":" << val.method() << std::endl;
        }
    }
    return MethodRegistry();
}
void ClassRegistry::newMethod(const char *signature, bool isStatic, MSCExternMethodFn value)
{
    auto key = (isStatic ? "static " : "") + std::string(signature);
    std::cout << "Registering method " << signature << " " << key << ":" << value << std::endl;
    _methods.insert(std::pair<std::string, MethodRegistry>(key, MethodRegistry(signature, isStatic, value)));
}
ModuleRegistry::ModuleRegistry(const char *name, const char **source) : _name(name), _source(source), _classes()
{
}

ClassRegistry *ModuleRegistry::clazz(const char *name)
{
    auto key = std::string(name);
    auto found = _classes.find(key);
    return found != _classes.end() ? &(found->second) : nullptr;
}
void ModuleRegistry::newClass(ClassRegistry &clazz)
{
    _classes.insert(std::pair<std::string, ClassRegistry>(clazz.name(), clazz));
}

PackageRegistry::PackageRegistry(const char *name) : _name(name), _modules() {}

void PackageRegistry::newModule(ModuleRegistry &module)
{
    _modules.insert(std::pair<std::string, ModuleRegistry>(module.name(), module));
}
ModuleRegistry PackageRegistry::module(const char *name)
{
    auto key = std::string(name);
    std::cout << "Finding module: " << name << std::endl;
    return _modules[key];
}

MoscRuntime::MoscRuntime(MSCConfig *config) : _rootDirectory(nullptr), _moduleDirectory(nullptr), _config(config), bindClassFn(nullptr), bindMethodFn(nullptr), _exitCode(0), handles()
{
    _config->bindExternClassFn = [](MVM *vm, const char *module, const char *className) -> MSCExternClassMethods
    {
        MoscRuntime *thisRuntime = (MoscRuntime *)MSCGetUserData(vm);
        auto methods = thisRuntime->bindBuiltInExternClass(module, className);
        if (methods.allocate != nullptr)
            return methods;

        if (thisRuntime->bindClassFn != nullptr)
        {
            return thisRuntime->bindClassFn(vm, module, className);
        }
        return methods;
    };
    _config->bindExternMethodFn = [](MVM *vm, const char *module,
                                     const char *className, bool isStatic, const char *signature) -> MSCExternMethodFn
    {
        MoscRuntime *thisRuntime = (MoscRuntime *)MSCGetUserData(vm);
        MSCExternMethodFn method = thisRuntime->bindBuiltInExternMethod(module, className,
                                                                        isStatic, signature);

        // std::cout<<"Fetching method:::: "<<(isStatic ? "static" : "")<<signature<<method<<std::endl;
        if (method != nullptr)
        {
            return method;
        }

        if (thisRuntime->bindMethodFn != nullptr)
        {
            return thisRuntime->bindMethodFn(vm, module, className, isStatic, signature);
        }

        return nullptr;
    };
    _config->resolveModuleFn = [](MVM *vm, const char *importer,
                                  const char *module) -> const char *
    {
        MoscRuntime *thisRuntime = (MoscRuntime *)MSCGetUserData(vm);
        auto resolved = thisRuntime->resolveModule(importer, module);
        return strdup(resolved.c_str());
    };
    _config->loadModuleFn = [](MVM *vm, const char *module) -> MSCLoadModuleResult
    {
        MoscRuntime *thisRuntime = (MoscRuntime *)MSCGetUserData(vm);
        MSCLoadModuleResult result = {0};
        std::unique_ptr<Path> filePath;
        auto ptype = Path::type(module);
        if (ptype == PATH_TYPE_SIMPLE)
        {
            // If there is no "mosc_libs" directory, then the only logical imports
            // we can handle are built-in ones. Let the VM try to handle it.

            if (thisRuntime->_moduleDirectory == nullptr)
                return thisRuntime->loadBuiltInModule(module);

            // TODO: Should we explicitly check for the existence of the module's base
            // directory inside "mosc_libs" here?

            // Look up the module in "mosc_libs".
            filePath = std::make_unique<Path>(thisRuntime->_moduleDirectory->toString());
            filePath->join(module);

            // If the module is a single bare name, treat it as a module with the same
            // name inside the package. So "foo" means "foo/foo".
            if (strchr(module, '/') == nullptr)
                filePath->join(module);
        }
        else
        {
            // The module path is already a file path.
            filePath = std::make_unique<Path>(module);
        }

        // Add a ".msc" file extension.
        filePath->appendString(".msc");
        std::unique_ptr<std::string> content = ResourceManager::readSystemFile(filePath->toString());
        result.source = strdup(content->c_str());

        result.onComplete = [](MVM *vm, const char *name, struct MSCLoadModuleResult result)
        {
            // clean up if required
            if (result.source)
            {
                free((void *)result.source); // Clean up the copied string
            }
        };
        return result;
    };
    _config->userData = this;
    _vm = MSCNewVM(_config);
}
// std::map<std::string, MSCHandle *> MoscRuntime::handles = std::map<std::string, MSCHandle *>();

MSCHandle *MoscRuntime::makeCallHandle(const char *signature) {
    auto key = signature;
     MSCHandle *handle = (handles.find(key) != handles.end()) ? handles.at(key) : nullptr;
     if (handle)
    {
        return handle;
    }
    handle = MSCMakeCallHandle(_vm, signature);
     handles[key] = handle;
    return handle;
}
MSCHandle *MoscRuntime::makeHandle(Djuru *djuru, int slot, const char *module, const char *name)
{
    auto key = std::string(module) + "." + name;
    MSCHandle *handle = (handles.find(key) != handles.end()) ? handles.at(key) : nullptr;
    if (handle)
    {
        return handle;
    }
    MSCGetVariable(djuru, module, name, 0);
    handle = MSCGetSlotHandle(djuru, 0);
    handles[key] = handle;
    return handle;
}

MoscRuntime::~MoscRuntime()
{
     for (auto const &handle : handles)
    {
        if (handle.second)
        {
            MSCReleaseHandle(_vm, handle.second);
        }
    }
    MSCFreeVM(_vm);
}
std::vector<PackageRegistry> MoscRuntime::packages = std::vector<PackageRegistry>();

MSCLoadModuleResult MoscRuntime::loadBuiltInModule(const char *name)
{
    MSCLoadModuleResult result = {0};
    auto mod = this->module(name);
    if (mod.source() == nullptr)
        return result;
    const char *module = strdup(*mod.source());

    result.onComplete = [](MVM *vm, const char *name, struct MSCLoadModuleResult result)
    {
        // clean up if required
        if (result.source)
        {
            free((void *)result.source); // Clean up the copied string
        }
    };

    result.source = module;
    return result;
}
void MoscRuntime::findModulesDirectory()
{
    if (_moduleDirectory != nullptr)
        return;

    std::unique_ptr<Path> searchDirectory = std::make_unique<Path>(_rootDirectory);
    std::unique_ptr<Path> lastPath = std::make_unique<Path>(ResourceManager::realPath(searchDirectory->toString()));
    if (!lastPath)
        return;
    // Keep walking up directories as long as we find them.
    for (;;)
    {
        std::unique_ptr<Path> modulesDirectory = std::make_unique<Path>(searchDirectory->toString());
        modulesDirectory->join("mosc_libs");

        if (ResourceManager::isDirectory(modulesDirectory->toString()))
        {
            modulesDirectory->normalize();
            _moduleDirectory = std::move(modulesDirectory);
            break;
        }

        // Walk up directories until we hit the root. We can tell that because
        // adding ".." yields the same real path.
        searchDirectory->join("..");
        std::unique_ptr<Path> thisPath = std::make_unique<Path>(ResourceManager::realPath(searchDirectory->toString()));
        if (!lastPath)
            break;
        if (*lastPath == *thisPath)
        {
            break;
        }
        lastPath = std::move(thisPath);
    }
}

void MoscRuntime::install(PackageRegistry package)
{
    packages.push_back(package);
}
const char *MoscRuntime::moduleSource(const char *name)
{
    for (auto pack : packages)
    {
        auto mod = pack.module(name);
        if (mod.source() != nullptr)
        {
            return strdup(*mod.source());
        }
    }
    return nullptr;
}
ModuleRegistry MoscRuntime::module(const char *name)
{
    for (auto pack : packages)
    {
        auto mod = pack.module(name);
        if (mod.isValid())
        {
            return mod;
        }
    }
    return ModuleRegistry();
}
MSCExternMethodFn MoscRuntime::bindBuiltInExternMethod(const char *name,
                                                       const char *className, bool isStatic, const char *signature)
{
    ModuleRegistry module = this->module(name);
    if (!module.isValid())
        return nullptr;
    ClassRegistry *clazz = module.clazz(className);
    if (clazz == nullptr)
        return nullptr;

    MethodRegistry method = clazz->method(signature, isStatic);
    std::cout << "BIN" << method.method() << std::endl;
    return method.method();
}
MSCExternClassMethods MoscRuntime::bindBuiltInExternClass(const char *moduleName, const char *className)
{
    MSCExternClassMethods methods = {nullptr, nullptr};

    ModuleRegistry module = this->module(moduleName);
    if (module.source() == nullptr)
        return methods;

    ClassRegistry *clazz = module.clazz(className);
    if (clazz == nullptr)
        return methods;

    MethodRegistry method = clazz->method("<allocate>", true);
    methods.allocate = method.method();
    method = clazz->method("<finalize>", true);
    methods.finalize = (MSCFinalizerFn)method.method();
    std::cout << "Class bound" << methods.finalize << "vs" << methods.allocate << std::endl;
    return methods;
}
std::string MoscRuntime::resolveModule(const char *importer, const char *name)
{
    if (Path::type(name) == PATH_TYPE_SIMPLE)
        return std::string(name);

    // Get the directory containing the importing module.
    std::unique_ptr<Path> path = std::make_unique<Path>(importer);

    return path->dirName()->join(name)->normalize()->toString();
}
InterpretResult MoscRuntime::run(const char *root, std::string module, std::string source)
{
    _rootDirectory = std::move(root);
    findModulesDirectory();
    return MSCInterpret(_vm, module.c_str(), source.c_str());
}
InterpretResult MoscRuntime::runFile(const char *file)
{
    auto source = ResourceManager::readSystemFile(file);
    if (!source)
    {
        std::cerr << "File not found" << std::endl;
        exit(65);
    }
    auto module = std::make_unique<Path>(file);
    if (Path::type(file) == PATH_TYPE_SIMPLE)
    {
        auto relative = std::make_unique<Path>(".");
        relative->join(file);
        module = std::move(relative);
    }
    module->removeExtension();
    // Use the directory where the file is as the root to resolve imports
    // relative to.
    auto directory = std::make_unique<Path>(module->toString());
    _rootDirectory = directory->dirName()->toString();

    findModulesDirectory();
    return MSCInterpret(_vm, module->toString(), source->c_str());
}

ModuleBuilder PackageBuilder::module(const char *name, const char **source)
{

    return ModuleBuilder(*this, name, source);
}
PackageBuilder &PackageBuilder::provide(ModuleProvider &provider)
{
    ModuleBuilder builder = ModuleBuilder(*this, provider.moduleName(), provider.moduleSource());
    provider.provide(builder);
    return *this;
}
ClassBuilder ModuleBuilder::clazz(const char *name)
{
    return ClassBuilder(*this, name);
}
ModuleBuilder &ModuleBuilder::provide(ClassProvider &provider)
{
    auto builder = ClassBuilder(*this, provider.className());
    provider.provide(builder);
    return *this;
}