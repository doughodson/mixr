
#ifndef __mixr_graphics_Material_HPP__
#define __mixr_graphics_Material_HPP__

#include "mixr/base/Component.hpp"
#include "mixr/base/osg/Vec4d"

#include "mixr/base/util/platform_api.hpp"
#include <GL/glu.h>

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Material
//
// Description: Object which will hold materialistic information, which can then
// have light applied to it.
//   Ambient color - light coming from material scattered in all directions equally
//   Diffuse color - light coming from the material after light hits it (main color)
//   Emissive color - light emanating from the material
//   Specular color - mirror reflecting color of the material (shiny highlights)
//   How shiny our material is (0 = dull, 128 = really shiny)
//
// Factory name: Material
// Slots
//    ambientColor   <Pairstream>  ! List of numbers for the ambient values
//    ambientColor   <INumber>     ! Ambient color - light coming from material scattered in all directions equally
//                                 (default: (0,0,0,1))
//    diffuseColor   <Pairstream>  ! List of numbers for the diffuse values
//    diffuseColor   <INumber>     ! Diffuse color - light coming from the material after light hits it (main color)
//                                 (default: (1,1,1,1))
//    emissiveColor  <Pairstream>  ! List of numbers for the emissive values
//    emissiveColor  <INumber>     ! Emissive color - light emanating from the material
//                                 (default: (0,0,0,1))
//    specularColor  <Pairstream>  ! List of numbers for the specular values
//    specularColor  <INumber>     ! Specular color - mirror reflecting color of the material (shiny highlights)
//                                 (default: (0,0,0,1))
//    shininess      <INumber>     ! How shiny our material is (0 = dull, 128 = really shiny) (default: 0)
//------------------------------------------------------------------------------
class Material : public base::Component
{
    DECLARE_SUBCLASS(Material, base::Component)

public:
    Material();

    // set data
    virtual bool setAmbientColor(base::Vec4d x);
    virtual bool setAmbientColor(double x, double y, double z, double w = 1.0);
    virtual bool setDiffuseColor(base::Vec4d x);
    virtual bool setDiffuseColor(double x, double y, double z, double w = 1.0);
    virtual bool setEmissiveColor(base::Vec4d x);
    virtual bool setEmissiveColor(double x, double y, double z, double w = 1.0);
    virtual bool setSpecularColor(base::Vec4d x);
    virtual bool setSpecularColor(double x, double y, double z, double w = 1.0);
    virtual bool setShininess(double x);

    // get data
    base::Vec4d& getAmbientColor()       { return ambientColor; }
    base::Vec4d& getDiffuseColor()       { return diffuseColor; }
    base::Vec4d& getEmissiveColor()      { return emissiveColor; }
    base::Vec4d& getSpecularColor()      { return specularColor; }
    double getShininess()                { return shininess; }

private:
    base::Vec4d ambientColor;     // ambient color
    base::Vec4d diffuseColor;     // diffuse color
    base::Vec4d emissiveColor;    // emissive color
    base::Vec4d specularColor;    // specular color
    double shininess {};          // shininess value (default: no shine)

private:
    // slot table helper methods
    bool setSlotAmbientColor(const base::PairStream* const);
    bool setSlotAmbientColor(const base::INumber* const);
    bool setSlotDiffuseColor(const base::PairStream* const);
    bool setSlotDiffuseColor(const base::INumber* const);
    bool setSlotEmissiveColor(const base::PairStream* const);
    bool setSlotEmissiveColor(const base::INumber* const);
    bool setSlotSpecularColor(const base::PairStream* const);
    bool setSlotSpecularColor(const base::INumber* const);
    bool setSlotShininess(const base::INumber*const);
};

}
}

#endif
