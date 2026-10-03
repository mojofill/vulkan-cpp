[[vk::binding(1, 0)]]
[[vk::image_format("rgba32f")]]
RWTexture2D<float4> outputImage;

struct FragmentInput
{
    float2 outNDC : TEXCOORD0;
};

struct FragmentOutput
{
    float4 outColor : SV_Target0;
};

[shader("fragment")]
FragmentOutput main(FragmentInput input)
{
    FragmentOutput output;

    float2 uv = input.outNDC * 0.5 + 0.5;

    uint width;
    uint height;
    outputImage.GetDimensions(width, height);

    int2 texCoord = int2(uv * float2(width, height));

    output.outColor = outputImage.Load(texCoord);

    return output;
}