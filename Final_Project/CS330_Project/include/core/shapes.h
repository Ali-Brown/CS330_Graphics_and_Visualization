#pragma once

#include <vector>
#include <numbers>
#include <tuple>
#include <rendering/types.h>


struct Shapes {
    static inline void UpdateNormals(Vertex& p1, Vertex& p2, Vertex& p3) {
        glm::vec3 U = p2.Position - p1.Position;
        glm::vec3 V = p3.Position - p1.Position;

        auto normal = glm::cross(U, V);

        p1.Normal = normal;
        p2.Normal = normal;
        p3.Normal = normal;
    };

    static inline std::vector<Vertex> GetUnitCircleVertices(uint32_t sectorCount)
    {
        //const float PI = 3.1415926f;
        float sectorStep = 2.f * std::numbers::pi_v<float> / static_cast<float>(sectorCount);
        float sectorAngle;  // radian

        std::vector<Vertex> vertices;

        for (auto i = 0; i <= sectorCount; i++)
        {
            sectorAngle = static_cast<float>(i) * sectorStep;

            vertices.push_back({
                .Position = {
                    std::cos(sectorAngle), // x
                    std::sin(sectorAngle), // y
                    0 // z
                }
                });

        }
        return vertices;
    };


    // adapted from: https://www.songho.ca/opengl/gl_cylinder.html
    static inline std::tuple<std::vector<Vertex>, std::vector<uint32_t>> BuildCylinderSmooth(uint32_t sectorCount, float baseRadius, float height) {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        // get unit circle vectors on XY-plane
        auto unitCircleVertices = GetUnitCircleVertices(sectorCount);

        /*VERTEX ARRAY*/

        // put side vertices to arrays
        for (auto i = 0; i < 2; ++i) {
            float h = -height / 2.f + static_cast<float>(i) * height;           // z value; -h/2 to h/2
            float t = 1.f - i;                              // vertical tex coord; 1 to 0

            //loop over all unit circle vertices
            uint32_t vertexIndex{ 0 };

            for (auto vertex : unitCircleVertices) {
                vertices.push_back({
                    .Position = {
                        vertex.Position.x * baseRadius,
                        vertex.Position.y * baseRadius,
                        h
                    },
                    .Color = {
                        vertexIndex % 3 == 0.f ? 1.f : 0.f,
                        vertexIndex % 3 == 0.f ? 1.f : 0.f,
                        vertexIndex % 3 == 0.f ? 1.f : 0.f,
                    },
                    .Normal = {
                        vertex.Position.x * baseRadius,
                        vertex.Position.y * baseRadius,
                        vertex.Position.z * baseRadius,
                    },
                    .Uv = {
                        static_cast<float>(vertexIndex) / static_cast<float>(sectorCount),
                        t
                    }
                });

                vertexIndex++;
            }
        }

        auto baseCenterIndex = vertices.size();
        auto topCenterIndex = baseCenterIndex + sectorCount + 2;

        //Build the top and bottom
        for (auto i = 0; i < 2; i++) {
            float h = -height / 2.f + static_cast<float>(i) * height; // z value; -h/2 to h/2
            float nz = -1.f + static_cast<float>(i) * 2;    // z value of normal; -1 to 1

            //center point
            vertices.push_back({
                .Position = {0, 0, h},
                .Color = {1.f, 1.f, 1.f},
                .Normal = {0, 0, nz},
                .Uv = {0.5f, 0.5f}
            });
            
            uint32_t vertexIndex{ 0 };
            for (auto vertex : unitCircleVertices) {
                vertices.push_back({
                        .Position = {
                            vertex.Position.x * baseRadius,
                            vertex.Position.y * baseRadius,
                            h
                        },
                        .Color = {
                            vertexIndex % 3 == 0.f ? 1.f : 0.f,
                            vertexIndex % 3 == 0.f ? 1.f : 0.f,
                            vertexIndex % 3 == 0.f ? 1.f : 0.f,
                        },
                        .Normal = {
                            0,
                            0,
                            nz,
                        },
                        .Uv = {
                            -vertex.Position.x * 0.5f + 0.5f,
                            -vertex.Position.x * 0.5f + 0.5f
                        }
                    });

                vertexIndex++;
            }
        }

        /*ELEMENTS INDEX ARRAY*/
        uint32_t k1 = 0;
        uint32_t k2 = sectorCount + 1;

        //side surface
        for (auto i = 0; i < sectorCount; i++, k1++, k2++) {
            // 2 triangles per sector
            indices.push_back(k1);
            indices.push_back(k1 + 1);
            indices.push_back(k2);

            indices.push_back(k2);
            indices.push_back(k1 + 1);
            indices.push_back(k2 + 1);
        }

        // bottom surface
        for (uint32_t i = 0, k = baseCenterIndex; i < sectorCount; i++, k++) {
            if (i < sectorCount -1) {
                indices.push_back(baseCenterIndex);
                indices.push_back(k + 1);
                indices.push_back(k);
            }
            else {
                indices.push_back(baseCenterIndex);
                indices.push_back(baseCenterIndex + 1);
                indices.push_back(k);
            }
        }

        // top surface
        for (uint32_t i = 0, k = topCenterIndex; i < sectorCount; i++, k++) {
            if (i < sectorCount - 1) {
                indices.push_back(topCenterIndex);
                indices.push_back(k);
                indices.push_back(k + 1);
            }
            else {
                indices.push_back(topCenterIndex);
                indices.push_back(k);
                indices.push_back(topCenterIndex + 1);
            }
        }

        return { vertices, indices };
     };


    static inline std::vector<Vertex> cubeVertices{
        // front face - red; facing +z
         {
            .Position = {-0.5f, 0.5f, 0.5f}, // 0
            .Color = {1.f, 0.f, 0.f},
            //.Normal = {0.f, 0.f, 1.f},
            .Uv = {0.f, 1.f}
        },
        {
            .Position = {-0.5f, -0.5f, 0.5f}, // 1
            .Color = {1.f, 0.f, 0.f},
            //.Normal = {0.f, 0.f, 1.f},
            .Uv = {0.f, 0.f}
        },
        {
            .Position = {0.5f, -0.5f, 0.5f}, // 2
            .Color = {1.f, 0.f, 0.f},
            //.Normal = {0.f, 0.f, 1.f},
            .Uv = {1.f, 0.f}
        },
        {
            .Position = {0.5f, 0.5f, 0.5f}, // 3
            .Color = {1.f, 0.f, 0.f},
            //.Normal = {0.f, 0.f, 1.f},
            .Uv = {1.f, 1.f}
        },
        // right face - green; facing +x
        {
            .Position = {0.5f, 0.5f, 0.5f}, // 4
            .Color = {0.f, 1.f, 0.f},
            //.Normal = {1.f, 0.f, 0.f},
            .Uv = {0.f, 1.f}
        },
        {
            .Position = {0.5f, -0.5f, 0.5f}, // 5
            .Color = {0.f, 1.f, 0.f},
            //.Normal = {1.f, 0.f, 0.f},
            .Uv = {0.f, 0.f}
        },
        {
            .Position = {0.5f, -0.5f, -0.5f}, // 6
            .Color = {0.f, 1.f, 0.f},
            //.Normal = {1.f, 0.f, 0.f},
            .Uv = {1.f, 0.f}
        }
        ,
        {
            .Position = {0.5f, 0.5f, -0.5f}, // 7
            .Color = {0.f, 1.f, 0.f},
            //.Normal = {1.f, 0.f, 0.f},
            .Uv = {1.f, 1.f}
        },
        // back face - cyan; facing -z
        {
            .Position = {0.5f, 0.5f, -0.5f}, // 8
            .Color = {0.f, 1.f, 1.f},
            //.Normal = {0.f, 0.f, -1.f},
            .Uv = {0.f, 1.f}
        },
        {
            .Position = {0.5f, -0.5f, -0.5f}, // 9
            .Color = {0.f, 1.f, 1.f},
            //.Normal = {0.f, 0.f, -1.f},
            .Uv = {0.f, 0.f}
        },
        {
            .Position = {-0.5f, -0.5f, -0.5f}, // 10
            .Color = {0.f, 1.f, 1.f},
            //.Normal = {0.f, 0.f, -1.f},
            .Uv = {1.f, 0.f}
        }
        ,
        {
            .Position = {-0.5f, 0.5f, -0.5f}, // 11
            .Color = {0.f, 1.f, 1.f},
            //.Normal = {0.f, 0.f, -1.f},
            .Uv = {1.f, 1.f}
        },
        // left face - magenta; facing -x
        {
            .Position = {-0.5f, 0.5f, -0.5f}, // 12
            .Color = {1.f, 0.f, 1.f},
            //.Normal = {-1.f, 0.f, 0.f},
            .Uv = {0.f, 1.f}
        },
        {
            .Position = {-0.5f, -0.5f, -0.5f}, // 13
            .Color = {1.f, 0.f, 1.f},
            //.Normal = {-1.f, 0.f, 0.f},
            .Uv = {0.f, 0.f}
        },
        {
            .Position = {-0.5f, -0.5f, 0.5f}, // 14
            .Color = {1.f, 0.f, 1.f},
            //.Normal = {-1.f, 0.f, 0.f},
            .Uv = {1.f, 0.f}
        }
        ,
        {
            .Position = {-0.5f, 0.5f, 0.5f}, // 15
            .Color = {1.f, 0.f, 1.f},
            //.Normal = {-1.f, 0.f, 0.f},
            .Uv = {1.f, 1.f}
        },
        // top face - yellow; facing +y
        {
            .Position = {-0.5f, 0.5f, -0.5f}, // 16
            .Color = {1.f, 1.f, 0.f},
            //.Normal = {0.f, 1.f, 0.f},
            .Uv = {0.f, 1.f}
        },
        {
            .Position = {-0.5f, 0.5f, 0.5f}, // 17
            .Color = {1.f, 1.f, 0.f},
            //.Normal = {0.f, 1.f, 0.f},
            .Uv = {0.f, 0.f}
        },
        {
            .Position = {0.5f, 0.5f, 0.5f}, // 18
            .Color = {1.f, 1.f, 0.f},
            //.Normal = {0.f, 1.f, 0.f},
            .Uv = {1.f, 0.f}
        }
        ,
        {
            .Position = {0.5f, 0.5f, -0.5f}, // 19
            .Color = {1.f, 1.f, 0.f},
            //.Normal = {0.f, 1.f, 0.f},
            .Uv = {1.f, 1.f}
        },
        // bottom face - blue; facing -y
        {
            .Position = {0.5f, -0.5f, 0.5f}, // 20
            .Color = {0.0f, 0.0f, 1.0f},
            //.Normal = {0.f, -1.f, 0.f},
            .Uv = {0.f, 1.f}
        },
        {
            .Position = {0.5f, -0.5f, -0.5f}, // 21
            .Color = {0.0f, 0.0f, 1.0f},
            //.Normal = {0.f, -1.f, 0.f},
            .Uv = {0.f, 0.f}
        },
        {
            .Position = {-0.5f, -0.5f, -0.5f}, // 22
            .Color = {0.0f, 0.0f, 1.0f},
            //.Normal = {0.f, -1.f, 0.f},
            .Uv = {1.f, 0.f}
        }
        ,
        {
            .Position = {-0.5f, -0.5f, 0.5f}, // 23
            .Color = {0.0f, 0.0f, 1.0f},
            //.Normal = {0.f, -1.f, 0.f},
            .Uv = {1.f, 1.f}
        }
    };

    static inline std::vector<uint32_t> cubeElements{
        0, 1, 3, // front face
        1, 2, 3,
        4, 5, 7, // right face
        5, 6, 7,
        8, 9, 11, // back face
        9, 10, 11,
        12, 13, 15, // left face
        13, 14, 15,
        16, 17, 19, // top face
        17, 18, 19,
        20, 21, 23, // bottom face
        21, 22, 23
    };

    static inline std::vector<Vertex> planeVertices{
        // lone face - blue
       {
           .Position = {0.5f, -0.5f, 0.5f}, // 0
           .Color = {0.0f, 0.0f, 1.0f},
           .Uv = {0.f, 1.f}
       },
       {
           .Position = {0.5f, -0.5f, -0.5f}, // 1
           .Color = {0.0f, 0.0f, 1.0f},
           .Uv = {0.f, 0.f}
       },
       {
           .Position = {-0.5f, -0.5f, -0.5f}, // 2
           .Color = {0.0f, 0.0f, 1.0f},
           .Uv = {1.f, 0.f}
       }
       ,
       {
           .Position = {-0.5f, -0.5f, 0.5f}, // 3
           .Color = {0.0f, 0.0f, 1.0f},
           .Uv = {1.f, 1.f}
       }
    };

    static inline std::vector<uint32_t> planeElements{
        0, 1, 3, // front face
        1, 2, 3,
    };

    static inline std::vector<Vertex> pyramidVertices{
        // FRONT TRI
       {
           .Position = {-0.5f, 0.f, 0.5f}, // 2
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {-1.f, 0.f, 0.f},
           .Uv = {0.f, 0.f}
       },
       {
           .Position = {0.f, 1.f, 0.0f}, // 1
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {-1.f, 0.f, 0.f},
           .Uv = {0.5f, 1.f}
       },
       {
           .Position = {0.5f, 0.f, 0.5f}, // 0
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {-1.f, 0.f, 0.f},
           .Uv = {1.f, 0.f}
       }
       ,
       //RIGHT TRI
       {
           .Position = {0.5f, 0.f, 0.5f}, // 5
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, 0.f, 1.f},
           .Uv = {0.f, 0.f}
       },
       {
           .Position = {0.f, 1.f, 0.f}, // 4
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, 0.f, 1.f},
           .Uv = {0.5f, 1.f}
       },
       {
           .Position = {0.5f, 0.f, -0.5f}, // 3
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, 0.f, 1.f},
           .Uv = {1.f, 0.f}
       },
        //BACK TRI
        {
            .Position = {0.5f, 0.f, -0.5f}, // 8
            .Color = {0.0f, 0.0f, 1.0f},
            //.Normal = {0.f, 0.f, 1.f},
            .Uv = {0.f, 0.f}
        },
        {
            .Position = {0.f, 1.f, 0.f}, // 7
            .Color = {0.0f, 0.0f, 1.0f},
            //.Normal = {0.f, 0.f, 1.f},
            .Uv = {0.5f, 1.f}
        },
        {
            .Position = {-0.5f, 0.f, -0.5f}, // 6
            .Color = {0.0f, 0.0f, 1.0f},
            //.Normal = {0.f, 0.f, 1.f},
            .Uv = {1.f, 0.f}
        },
        //LEFT TRI
       {
           .Position = {-0.5f, 0.f, -0.5f}, // 11
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, 0.f, 1.f},
           .Uv = {0.f, 0.f}
       },
       {
           .Position = {0.f, 1.f, 0.f}, // 10
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, 0.f, 1.f},
           .Uv = {0.5f, 1.f}
       },
       {
           .Position = {-0.5f, 0.f, 0.5f}, // 9
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, 0.f, 1.f},
           .Uv = {1.f, 0.f}
       },

        //BOTTOM QUAD
       {
           .Position = {-0.5f, 0.f, -0.5f}, // 14
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, -1.f, 0.f},
           .Uv = {0.f, 0.f}
       },
       {
           .Position = {-0.5f, 0.f, 0.5f}, // 13
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, -1.f, 0.f},
           .Uv = {0.f, 1.f}
       },
       {
           .Position = {0.5f, 0.f, -0.5f}, // 12
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, -1.f, 0.f},
           .Uv = {1.f, 0.f}
       },
       {
           .Position = {0.5f, 0.f, 0.5f}, // 15
           .Color = {0.0f, 0.0f, 1.0f},
           //.Normal = {0.f, -1.f, 0.f},
           .Uv = {1.f, 1.f}
       }
    };

    static inline std::vector<uint32_t> pyramidElements{
        2, 1, 0, // front tri face
        5, 4, 3, // right tri face
        8, 7, 6, // back tri face
        11, 10, 9, // left tri face
        14, 13, 12, // bottom quad faces
        14, 15, 13
    };

};