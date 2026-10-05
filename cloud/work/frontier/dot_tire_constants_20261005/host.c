/* Executes the unchanged candidate under ordinary host C89 and UBSan. */
#include <stddef.h>
#include <string.h>
#include CANDIDATE_SOURCE
#define ASSERT_LAYOUT(n,e) typedef char n[(e)?1:-1]
ASSERT_LAYOUT(model_size,sizeof(Model)==2056);
ASSERT_LAYOUT(tire_size,sizeof(Tire)==92);
ASSERT_LAYOUT(model_tires,offsetof(Model,tires)==1072);
ASSERT_LAYOUT(model_load,offsetof(Model,load)==1468);
ASSERT_LAYOUT(model_wheelbase,offsetof(Model,wheelbase)==1480);
ASSERT_LAYOUT(tire_stiffness,offsetof(Tire,Cstiff)==12);
ASSERT_LAYOUT(tire_cfmax,offsetof(Tire,Cfmax)==16);
ASSERT_LAYOUT(tire_load,offsetof(Tire,Zforce)==24);
ASSERT_LAYOUT(tire_patch,offsetof(Tire,patchy)==68);
void run_case(const void *input_model,const void *input_tire,float scale,int slot,
              void *output_model,void *output_tire)
{
    Model model;
    Tire separate;
    Tire *tire;
    memcpy(&model,input_model,sizeof(model));
    tire=slot<0?&separate:&model.tires[slot];
    memcpy(tire,input_tire,sizeof(*tire));
    track_preview_handler(&model,tire,scale);
    memcpy(output_tire,tire,sizeof(*tire));
    memcpy(output_model,&model,sizeof(model));
}
