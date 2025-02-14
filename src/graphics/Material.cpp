
#include "mixr/graphics/Material.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Material, "Material")
EMPTY_DELETEDATA(Material)

BEGIN_SLOTTABLE(Material)
    "ambientColor",         //  1: Ambient color - light coming from material scattered in all directions equally
    "diffuseColor",         //  2: Diffuse color - light coming from the material after light hits it (main color)
    "emissiveColor",        //  3: Emissive color - light emanating from the material
    "specularColor",        //  4: Specular color - mirror reflecting color of the material (shiny highlights)
    "shininess",            //  5: How shiny our material is (0 = dull, 128 = really shiny)
END_SLOTTABLE(Material)

BEGIN_SLOT_MAP(Material)
    ON_SLOT( 1, setSlotAmbientColor,  base::PairStream )
    ON_SLOT( 1, setSlotAmbientColor,  base::INumber)
    ON_SLOT( 2, setSlotDiffuseColor,  base::PairStream )
    ON_SLOT( 2, setSlotDiffuseColor,  base::INumber)
    ON_SLOT( 3, setSlotEmissiveColor, base::PairStream )
    ON_SLOT( 3, setSlotEmissiveColor, base::INumber)
    ON_SLOT( 4, setSlotSpecularColor, base::PairStream )
    ON_SLOT( 4, setSlotSpecularColor, base::INumber)
    ON_SLOT( 5, setSlotShininess,     base::INumber)
END_SLOT_MAP()

Material::Material()
{
    STANDARD_CONSTRUCTOR()
    ambientColor.set(0,0,0,1);      // no ambient color from the material
    diffuseColor.set(1,1,1,1);      // white light diffuse off of the material (this is the closest thing to actual material color)
    emissiveColor.set(0,0,0,1);     // no emissive color
    specularColor.set(0,0,0,1);     // no specular color
}

void Material::copyData(const Material& org, const bool)
{
    BaseClass::copyData(org);
    ambientColor = org.ambientColor;
    diffuseColor = org.diffuseColor;
    emissiveColor = org.emissiveColor;
    specularColor = org.specularColor;
    shininess = org.shininess;
}

//------------------------------------------------------------------------------
// setSlotAmbientColor() - give us a list of numbers (0 - 1) for the ambient
// values (red, green, blue, and alpha) of our material.
//------------------------------------------------------------------------------
bool Material::setSlotAmbientColor(const base::PairStream* const x)
{
    if (x != nullptr) {
        // go through and get our data from the pairstream, and set our ambient color
        const base::List::Item* item = x->getFirstItem();
        int count = 0;
        double temp[4] = { 1,1,1,1 };
        while (item != nullptr && count < 4) {
            const auto pair = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
            if (pair != nullptr) {
                const auto num = dynamic_cast<base::INumber*>(pair->object());
                if (num != nullptr) temp[count++] = num->asDouble();
            }
            item = item->getNext();
        }
        // if we only put in red, green, and blue, we will default to 1 on alpha
        if (count == 3) temp[3] = 1;
        // now set our vector
        ambientColor.set(temp[0], temp[1], temp[2], temp[3]);
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotAmbientColor() - set our ambient values all the same as the number
// given (ie - 1 would be white, 0 black, 0.5 somewhere in between)
// ALPHA here is always 1.
//------------------------------------------------------------------------------
bool Material::setSlotAmbientColor(const base::INumber* const x)
{
    if (x != nullptr) {
        const double temp{x->asDouble()};
        ambientColor.set(temp, temp, temp, 1);
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotDiffuseColor() - give us a list of numbers (0 - 1) for the diffuse
// values (red, green, blue, and alpha) of our material.
//------------------------------------------------------------------------------
bool Material::setSlotDiffuseColor(const base::PairStream* const x)
{
    if (x != nullptr) {
        // go through and get our data from the pairstream, and set our ambient color
        const base::List::Item* item{x->getFirstItem()};
        int count{};
        double temp[4] = { 1,1,1,1 };
        while (item != nullptr && count < 4) {
            const auto pair = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
            if (pair != nullptr) {
                const auto num = dynamic_cast<base::INumber*>(pair->object());
                if (num != nullptr) temp[count++] = num->asDouble();
            }
            item = item->getNext();
        }
        // if we only put in red, green, and blue, we will default to 1 on alpha
        if (count == 3) temp[3] = 1;
        // now set our vector
        diffuseColor.set(temp[0], temp[1], temp[2], temp[3]);
    }
    return true;
}
//------------------------------------------------------------------------------
// setSlotDiffuseColor() - set our diffuse values all the same as the number
// given (ie - 1 would be white, 0 black, 0.5 somewhere in between)
// ALPHA here is always 1.
//------------------------------------------------------------------------------
bool Material::setSlotDiffuseColor(const base::INumber* const x)
{
    if (x != nullptr) {
        double temp{x->asDouble()};
        diffuseColor.set(temp, temp, temp, 1);
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotEmissiveColor() - give us a list of numbers (0 - 1) for the emissive
// values (red, green, blue, and alpha) of our material.
//------------------------------------------------------------------------------
bool Material::setSlotEmissiveColor(const base::PairStream* const x)
{
    if (x != nullptr) {
        // go through and get our data from the pairstream, and set our ambient color
        const base::List::Item* item{x->getFirstItem()};
        int count{};
        double temp[4] = { 1,1,1,1 };
        while (item != nullptr && count < 4) {
            const auto pair = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
            if (pair != nullptr) {
                const auto num = dynamic_cast<base::INumber*>(pair->object());
                if (num != nullptr) temp[count++] = num->asDouble();
            }
            item = item->getNext();
        }
        // if we only put in red, green, and blue, we will default to 1 on alpha
        if (count == 3) temp[3] = 1;
        // now set our vector
        emissiveColor.set(temp[0], temp[1], temp[2], temp[3]);
    }
    return true;
}
//------------------------------------------------------------------------------
// setSlotEmissiveColor() - set our emissive values all the same as the number
// given (ie - 1 would be white, 0 black, 0.5 somewhere in between)
// ALPHA here is always 1.
//------------------------------------------------------------------------------
bool Material::setSlotEmissiveColor(const base::INumber* const x)
{
    if (x != nullptr) {
        const double temp{x->asDouble()};
        emissiveColor.set(temp, temp, temp, 1);
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotSpecularColor() - give us a list of numbers (0 - 1) for the specular
// values (red, green, blue, and alpha) of our material.
//------------------------------------------------------------------------------
bool Material::setSlotSpecularColor(const base::PairStream* const x)
{
    if (x != nullptr) {
        // go through and get our data from the pairstream, and set our ambient color
        const base::List::Item* item = x->getFirstItem();
        int count = 0;
        double temp[4] = { 1,1,1,1 };
        while (item != nullptr && count < 4) {
            const auto pair = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
            if (pair != nullptr) {
                const auto num = dynamic_cast<base::INumber*>(pair->object());
                if (num != nullptr) temp[count++] = num->asDouble();
            }
            item = item->getNext();
        }
        // if we only put in red, green, and blue, we will default to 1 on alpha
        if (count == 3) temp[3] = 1;
        // now set our vector
        specularColor.set(temp[0], temp[1], temp[2], temp[3]);
    }
    return true;
}
//------------------------------------------------------------------------------
// setSlotSpecularColor() - set our specular values all the same as the number
// given (ie - 1 would be white, 0 black, 0.5 somewhere in between)
// ALPHA here is always 1.
//------------------------------------------------------------------------------
bool Material::setSlotSpecularColor(const base::INumber* const x)
{
    if (x != nullptr) {
        const double temp{x->asDouble()};
        specularColor.set(temp, temp, temp, 1);
    }
    return true;
}
//------------------------------------------------------------------------------
// setSlotShininess() - sets the shininess of our material
//------------------------------------------------------------------------------
bool Material::setSlotShininess(const base::INumber* const x)
{
    if (x != nullptr) {
        shininess = x->asDouble();
    }
    return true;
}

//------------------------------------------------------------------------------
// setAmbientColor() - set our ambient color with an OSG or double
//------------------------------------------------------------------------------
bool Material::setAmbientColor(base::Vec4d x)
{
    ambientColor = x;
    return true;
}
bool Material::setAmbientColor(double x, double y, double z, double w)
{
    ambientColor.set(x, y, z, w);
    return true;
}
//------------------------------------------------------------------------------
// setDiffuseColor() - set our diffuse color with an OSG
//------------------------------------------------------------------------------
bool Material::setDiffuseColor(base::Vec4d x)
{
    diffuseColor = x;
    return true;
}
bool Material::setDiffuseColor(double x, double y, double z, double w)
{
    diffuseColor.set(x, y, z, w);
    return true;
}
//------------------------------------------------------------------------------
// setEmissiveColor() - set our emissive color with an OSG
//------------------------------------------------------------------------------
bool Material::setEmissiveColor(base::Vec4d x)
{
    emissiveColor = x;
    return true;
}
bool Material::setEmissiveColor(double x, double y, double z, double w)
{
    emissiveColor.set(x, y, z, w);
    return true;
}
//------------------------------------------------------------------------------
// setSpecularColor() - set our specular color with an OSG
//------------------------------------------------------------------------------
bool Material::setSpecularColor(base::Vec4d x)
{
    specularColor = x;
    return true;
}
bool Material::setSpecularColor(double x, double y, double z, double w)
{
    specularColor.set(x, y, z, w);
    return true;
}
//------------------------------------------------------------------------------
// setShininess() - set our shininess
//------------------------------------------------------------------------------
bool Material::setShininess(double x)
{
    shininess = x;
    return true;
}

}
}

