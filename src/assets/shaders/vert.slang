struct VertexInput {
    float2 inPos : POSITION;
};

struct VertexOutput {
    float2 outNDC : TEXCOORD0;
    float4 position : SV_Position; // this is important - this must always be SV_position
};

[[vk::binding(0, 0)]]
cbuffer UniformBufferObject {
    float angle;
}

[shader("vertex")]
VertexOutput main(VertexInput input) {
    VertexOutput output;

    output.outNDC = input.inPos;
    output.position = float4(input.inPos, 0.0, 1.0);

    return output;
}