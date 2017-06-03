#include "defines.h"
static float B2[512];// = {-0.481000f,0.960843f,-0.931794f,-0.088597f,0.873207f,-0.017130f,-0.964724f,-2.093097f,-0.662082f,-0.524752f,0.847696f,1.666526f,1.046458f,0.361784f,-0.918413f,-0.230536f,-0.588405f,-0.070200f,-0.763968f,-0.459577f,-1.097784f,-0.885405f,-1.947492f,2.293438f,-0.057162f,1.797745f,-1.627788f,1.486274f,-1.630869f,-0.662570f,-1.706783f,-0.002892f,0.624469f,-1.486414f,-1.268386f,-0.328096f,-0.338524f,-0.064920f,1.902068f,-0.717650f,-0.104357f,1.377382f,0.053466f,0.481675f,-0.889013f,-2.302433f,1.356986f,-0.745744f,-1.917776f,0.714323f,1.044003f,0.988676f,-1.238054f,0.269885f,1.533027f,-1.096403f,-2.119834f,-0.750122f,1.358772f,0.833520f,1.483672f,-0.169000f,-0.478136f,-0.726284f,1.359751f,-0.112533f,-1.787741f,-1.384980f,-0.835523f,0.181527f,0.336436f,-1.774501f,-0.841534f,0.785763f,-1.382586f,0.083480f,0.140284f,0.520319f,-0.300995f,-0.272929f,-0.677659f,-1.179676f,-1.554890f,1.304204f,0.553728f,0.183414f,1.842701f,-0.249230f,0.887509f,0.132393f,0.189628f,-1.640354f,-0.689004f,1.615884f,0.430728f,-0.906806f,-1.220864f,-0.204319f,-0.130228f,-1.265898f,-1.275205f,-0.324783f,0.105616f,0.595031f,0.567954f,1.029010f,-0.868285f,0.881055f,0.385367f,-0.239679f,-0.531620f,-0.083541f,1.265296f,1.140047f,-1.456267f,0.410955f,-1.150751f,0.625709f,-0.479921f,-1.188268f,-1.198644f,1.077760f,0.594950f,-0.794384f,0.708575f,-0.037788f,-0.281494f,-1.127303f,-1.391932f,0.375027f,-0.883433f,-0.329896f,-0.452933f,0.338618f,-1.081795f,-1.880700f,-0.324444f,-0.043785f,1.888165f,0.844894f,-1.820271f,-0.736234f,-0.591557f,-0.765132f,-0.028062f,0.841479f,1.125749f,-0.857391f,0.598295f,-0.827873f,1.724067f,-1.295591f,0.052103f,1.384485f,0.379333f,0.059984f,-0.828060f,0.471495f,-1.633739f,-0.671322f,1.175648f,0.362882f,0.699009f,-0.981634f,0.165596f,1.075160f,-1.428385f,-1.075049f,0.830499f,-0.030183f,1.150882f,0.822869f,1.202731f,0.354988f,-1.511932f,0.808539f,-0.788390f,-1.568173f,0.533689f,-1.620298f,-0.134051f,1.584145f,0.718667f,0.655262f,0.568059f,0.044679f,1.064937f,1.727099f,-0.985282f,-0.829967f,-1.364063f,0.016357f,1.215454f,-0.284418f,0.739283f,-0.773602f,0.498707f,0.164730f,-2.332211f,-0.442017f,-0.041823f,1.140604f,-1.358223f,-1.399727f,-0.052950f,0.971833f,-0.575575f,-0.963872f,0.691631f,-0.220047f,-0.725984f,0.196700f,0.663107f,0.269560f,1.768118f,-0.642986f,-0.107363f,1.372349f,-0.133746f,0.408484f,-2.658868f,2.000614f,0.936325f,-0.877766f,0.140321f,1.428406f,-0.700210f,-0.688865f,-0.600075f,0.708364f,-0.781166f,1.537484f,-0.158134f,-0.061280f,-1.132161f,0.753655f,2.219930f,-1.053724f,0.795524f,0.639665f,0.012082f,-0.817167f,1.597042f,0.498037f,-0.082984f,-0.584462f,-0.140811f,0.359599f,0.303513f,2.058435f,-0.569339f,-1.552813f,1.905560f,0.484788f,-0.140749f,0.387129f,0.195992f,1.002802f,-0.378602f,-1.232030f,-0.051376f,-0.481042f,-1.103676f,0.571376f,-0.904182f,-1.699644f,0.153306f,-0.574695f,1.850182f,-0.556432f,-0.306122f,0.306920f,-1.159384f,-0.440170f,0.520306f,1.080188f,1.329538f,1.192753f,0.138058f,-0.444342f,0.987257f,0.379802f,2.065531f,-0.135019f,0.672477f,-0.153236f,-0.271985f,0.151758f,0.073227f,1.067791f,0.453821f,-1.266130f,0.433648f,-0.274293f,0.354942f,-1.041360f,-1.117956f,-0.614101f,0.496659f,0.242496f,-1.084605f,-0.985396f,1.481095f,0.627590f,-0.584042f,0.866407f,2.000799f,-1.798953f,0.743092f,-1.192271f,-0.381350f,0.272699f,0.023093f,1.192143f,1.011977f,0.913350f,1.669867f,0.865566f,-0.853802f,0.472347f,0.079049f,-0.781254f,0.802899f,0.502024f,1.199698f,0.356911f,0.381456f,0.398627f,-0.117069f,0.048004f,-1.024739f,-0.157321f,0.539258f,0.022976f,-0.863374f,2.731243f,-0.081832f,0.345945f,0.317939f,-0.173476f,1.378464f,-1.495458f,-0.005349f,0.488637f,1.951104f,0.931032f,0.879339f,-0.510793f,-0.335150f,0.822101f,1.161712f,-0.570631f,0.508958f,-0.757588f,-0.221365f,0.313156f,-0.126294f,0.662755f,-1.527703f,-1.271524f,1.488260f,0.889197f,-2.228116f,-0.788399f,-0.382094f,-0.434730f,0.894271f,-0.322564f,0.889963f,-0.734360f,-0.763028f,0.103545f,-2.222596f,1.526590f,0.695738f,0.572300f,-0.340878f,1.086019f,-1.605652f,0.097249f,0.232912f,-0.783130f,0.319059f,0.850483f,-0.151955f,-0.404822f,0.492087f,-0.105237f,-0.309380f,-1.199103f,0.971610f,1.122907f,-0.089747f,-0.371665f,0.244596f,-0.501286f,0.723577f,0.062034f,0.553672f,0.648438f,0.905491f,0.329322f,1.055761f,-0.456645f,0.274514f,-0.501322f,-0.189283f,-0.233959f,1.003654f,-0.809755f,-0.974355f,0.127638f,-0.181659f,0.210041f,1.566445f,-1.011415f,-1.181162f,-1.659600f,-1.388210f,-0.641005f,0.569050f,0.444125f,0.224318f,-0.106648f,-0.633275f,0.597132f,0.518029f,0.773258f,0.167090f,1.410296f,1.418025f,0.852954f,0.138967f,0.677263f,0.658994f,0.899913f,-1.776847f,1.462391f,-0.507305f,0.825290f,0.523722f,-1.314165f,0.224227f,-0.980318f,-0.642447f,-1.050422f,-1.640451f,-0.677247f,-0.487188f,0.967883f,0.488095f,-1.098730f,0.357849f,-1.042238f,-1.409436f,-1.194425f,1.542936f,1.046538f,0.009723f,1.910191f,0.452580f,-0.373902f,-0.331192f,-1.849400f,0.056317f,-1.243850f,-0.701752f,-0.221444f,-1.371719f,0.847957f,-0.914267f,2.131920f,-2.091325f,-0.366545f,-1.390646f,-0.634882f,1.651669f,-1.136942f,-0.610004f,2.038121f,1.192618f,-2.645891f,-0.417149f,0.177919f,-1.322120f,-0.039713f,0.278490f,-0.573507f,1.271172f,-1.187979f,1.534415f,-0.130913f,0.344516f,0.160671f,-0.009983f,0.042138f,-0.398652f,0.664382f,-0.541609f,1.598405f,0.087356f,-0.400004f,-0.405778f,-0.939349f,0.911280f,0.076359f,-0.043897f,0.517313f,-0.574039f,-0.403537f,0.356438f,-1.236653f};
