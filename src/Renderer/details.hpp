#ifndef DMP_RENDERER_PODS_HPP
#define DMP_RENDERER_PODS_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include "../Manager.hpp"

namespace dmp
{
  struct QueueDetails
  {
    uint32_t index;
    VkQueueFamilyProperties props;
    float priority;
  };

  struct LayerDetails
  {
    bool enabled;
    std::vector<const char *> layers;
  };

  struct ExtensionDetails
  {
    std::vector<const char *> extensions;
  };

  struct PhysicalDeviceDetails
  {
    VkPhysicalDeviceFeatures features;
  };

  struct InstanceDetails
  {
    LayerDetails layers;
    ExtensionDetails extensions;
  };

  struct DeviceDetails
  {
    LayerDetails layers;
    ExtensionDetails extensions;
  };

  struct SwapchainDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
    VkSurfaceKHR surface;
    uint32_t width;
    uint32_t height;
  };

  struct GraphicsPipelineDataLayoutDetails
  {
    // vertex input
    std::vector<VkVertexInputBindingDescription> vertBindDescriptions;
    std::vector<VkVertexInputAttributeDescription> vertAttribDescriptions;

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    std::vector<VkPushConstantRange> pushConstantRanges;
  };

  struct GraphicsPipelineFixedFunctionDetails
  {
    // input assembly
    VkPrimitiveTopology primTopology;
    // rasterizer
    VkPolygonMode polygonMode;
    VkCullModeFlags cullMode;
    VkFrontFace frontFace;
    float lineWidth;
    // multisampling
  };
}

#endif
