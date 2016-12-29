#include "Shader.hpp"
#include <fstream>
#include <utility>

// std::map<const std::string, std::vector<char>> dmp::Shader::memo;

// std::vector<char> dmp::Shader::loadSPV(const std::string & path)
// {
//   std::ifstream fin(path, std::ios::binary);
//   fin.seekg(0, std::ios_base::end);
//   auto endPos = fin.tellg();
//   fin.seekg(0, std::ios_base::beg);

//   memo[path].resize(endPos);
//   fin.read(memo[path].data(), endPos);

//   fin.close();

//   auto bytecodeIter = memo.find(path);

//   expect("Failed to load SPV file!",
//          bytecodeIter != memo.end());

//   return bytecodeIter->second;
// }

// void dmp::Shader::loadShader(const Manager<VkDevice> & dev,
//                              const std::string & path)
// {
//   if (mValid)
//     {
//       mShaderModule.reset();
//       mValid = false;
//     }

//   std::vector<char> bytecode;
//   auto bytecodeIter = memo.find(path);

//   if (bytecodeIter == memo.end())
//     {
//       bytecode = loadSPV(path);
//     }
//   else
//     {
//       bytecode = bytecodeIter->second;
//     }

//   mShaderModule = {dev, vkDestroyShaderModule};

//   VkShaderModuleCreateInfo sci;

//   sci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//   sci.pNext = nullptr;
//   sci.flags = 0;
//   sci.codeSize = (uint32_t) bytecode.size();
//   sci.pCode = (uint32_t *) bytecode.data();

//   expect("Failed to create shader module!",
//          vkCreateShaderModule(dev,
//                               &sci,
//                               nullptr,
//                               &mShaderModule) == VK_SUCCESS);

//   mValid = true;
// }

// dmp::Shader::operator VkShaderModule() const
// {
//   expect ("attempt to cast invalid shader module to VkShaderModule!",
//           mValid);
//   return (VkShaderModule) mShaderModule;
// }
