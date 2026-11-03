#include <vector>
#include <string>
#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"

using android::base::GetProperty;

struct Variant {
    std::string model;
    std::string name;
    std::string marketname;
    std::string camera;
};

static const Variant variant_g60 = {
    "moto g(60)", "hanoip_g", "Motorola Moto G60", "108MP + 8MP + 2MP"
};

static const Variant variant_g40_fusion = {
    "moto g(40) fusion", "hanoip_in", "Motorola Moto G40 Fusion", "64MP + 8MP + 2MP"
};

void property_override(const std::string& prop, const std::string& value) {
    auto pi = (prop_info*) __system_property_find(prop.c_str());
    if (pi != nullptr) {
        __system_property_update(pi, value.c_str(), value.size());
    } else {
        __system_property_add(prop.c_str(), prop.size(), value.c_str(), value.size());
    }
}

void set_gapps_props(const std::string& model, const std::string& name) {
    const std::vector<std::string> partitions = {
        "", "bootimage.", "odm.", "product.", "system.", "system_ext.", "vendor."
    };

    for (const auto& part : partitions) {
        property_override("ro.product." + part + "model", model);
        property_override("ro.product." + part + "name", name);
    }
}

void vendor_load_properties() {
    std::string sku = GetProperty("ro.boot.hardware.sku", "");
    std::string device = GetProperty("ro.boot.device", "");
    std::string cam_variant = GetProperty("ro.boot.cam_variant", "");
    if (cam_variant.empty()) {
        cam_variant = GetProperty("ro.vendor.hw.cam_variant", "");
    }
    std::string imager = GetProperty("ro.boot.imager", "");
    if (imager.empty()) {
        imager = GetProperty("ro.vendor.hw.imager", "");
    }

    // Moto G40 Fusion detection:
    // - Hardware SKU: XT2147-1 / XT2147-2
    // - Device codename: hanoi
    // - Camera variant 2: 64MP rear (OV64B) + 16MP front (OV16A1Q)
    // - Imager tag: 64MP
    bool is_g40_fusion = (sku.find("2147") != std::string::npos ||
                          device == "hanoi" ||
                          cam_variant == "2" ||
                          imager == "64MP");

    Variant current_variant = is_g40_fusion ? variant_g40_fusion : variant_g60;

    set_gapps_props(current_variant.model, current_variant.name);

    property_override("ro.product.marketname", current_variant.marketname);
    property_override("ro.infinity.soc", "Snapdragon 732G");
    property_override("ro.infinity.camera", current_variant.camera);

    std::string carrier = GetProperty("ro.boot.carrier", "");
    if (!carrier.empty()) {
        property_override("ro.carrier", carrier);
        property_override("ro.vendor.carrier", carrier);
    }

    std::string radio = GetProperty("ro.boot.radio", "");
    if (!radio.empty()) {
        property_override("ro.vendor.hw.radio", radio);
        property_override("ro.vendor.boot.radio", radio);
    }

    if (!device.empty()) {
        property_override("ro.vendor.hw.device", device);
    }

    std::string dualsim = GetProperty("ro.boot.dualsim", "");
    if (!dualsim.empty()) {
        property_override("ro.vendor.hw.dualsim", dualsim);
        if (dualsim == "true") {
            property_override("persist.radio.multisim.config", "dsds");
            property_override("persist.vendor.radio.multisim.config", "dsds");
        }
    }

    std::string sku_variant = GetProperty("ro.boot.sku_variant", "");
    if (!sku_variant.empty()) {
        property_override("ro.vendor.hw.sku_variant", sku_variant);
    }
}
