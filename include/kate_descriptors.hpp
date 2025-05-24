#pragma once

#include <kate_device.hpp>

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace kate {

    class KATEDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(KATEDevice &device) : dsbuilder_device{device} {}

            Builder &addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<KATEDescriptorSetLayout> build() const;

        private:
            KATEDevice &dsbuilder_device;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        KATEDescriptorSetLayout(
            KATEDevice &device, 
            std::unordered_map<uint32_t,VkDescriptorSetLayoutBinding> bindings);

        ~KATEDescriptorSetLayout();
        KATEDescriptorSetLayout(const KATEDescriptorSetLayout &) = delete;
        KATEDescriptorSetLayout &operator=(const KATEDescriptorSetLayout &) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        KATEDevice &dsDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class KATEDescriptorWriter;
    };

    class KATEDescriptorPool {
    public:
        class Builder {
        public:
            Builder(KATEDevice &device) : dpbuilder_Device{device} {}

            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &setMaxSets(uint32_t count);
            std::unique_ptr<KATEDescriptorPool> build() const;

        private:
            KATEDevice &dpbuilder_Device;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        KATEDescriptorPool(
            KATEDevice &device,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~KATEDescriptorPool();
        KATEDescriptorPool(const KATEDescriptorPool &) = delete;
        KATEDescriptorPool &operator=(const KATEDescriptorPool &) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

        void resetPool();

    private:
        KATEDevice &dpDevice;
        VkDescriptorPool descriptorPool;

        friend class KATEDescriptorWriter;
    };

    class KATEDescriptorWriter {
    public:
        KATEDescriptorWriter(KATEDescriptorSetLayout &setLayout, KATEDescriptorPool &pool);

        KATEDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        KATEDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);

    private:
        KATEDescriptorSetLayout &setLayout;
        KATEDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}  // namespace KATE