[[vk::binding(1, 0)]]
[[vk::image_format("rgba32f")]]
RWTexture2D<float4> outputImage;

[shader("compute")]
[numthreads(16, 16, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID) {
    int2 coord = int2(dispatchThreadID.xy);

    int2 imgSize;
    outputImage.GetDimensions(imgSize.x, imgSize.y);

    float2 center = 0.5 * float2(imgSize);
    float radius = 0.25 * float(imgSize.x);

    float2 diff = float2(coord) - center;

    float4 color = float4(0, 0, 0, 0);

    if (dot(diff, diff) <= radius * radius)
    {
        color = float4(1, 1, 1, 1);
    }

    outputImage[coord] = color;
}