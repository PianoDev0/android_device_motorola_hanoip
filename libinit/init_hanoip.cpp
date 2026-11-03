#include <vector>
#include <string>
#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"

using android::base::GetProperty;

struct Variant {
    std::string sku;
    std::string model;
    std::string name;
    std::string marketname;
    std::string camera;
};

static const std::vector<Variant> variants = {
    {"XT2135-1", "moto g(60)", "hanoip_g", "Motorola Moto G60", "108MP + 8MP + 2MP"},
    {"XT2135-2", "moto g(60)", "hanoip_g", "Motorola Moto G60", "108MP + 8MP + 2MP"},
    {"XT2137-1", "moto g(40) fusion", "hanoip_in", "Motorola Moto G40 Fusion", "64MP + 8MP + 2MP"},
    {"XT2137-2", "moto g(40) fusion", "hanoip_in", "Motorola Moto G40 Fusion", "64MP + 8MP + 2MP"},
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

    Variant current_variant = variants[0];

    for (const auto& variant : variants) {
        if (variant.sku == sku) {
            current_variant = variant;
            break;
        }
    }

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

    std::string device = GetProperty("ro.boot.device", "");
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
