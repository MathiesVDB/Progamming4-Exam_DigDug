#pragma once
#include <memory>

#include "Component.h"
#include <string>
#include <SDL.h>

#include "Texture2D.h"

class GameObject;

class TextureComponent final : public Component
{
public:
    TextureComponent(std::string  filename);
    TextureComponent(const TextureComponent&) = delete;
    TextureComponent(TextureComponent&&) = delete;
    TextureComponent& operator= (const TextureComponent&) = delete;
    TextureComponent& operator= (const TextureComponent&&) = delete;

    void Render() const override;

    void SetTexture(const std::string& filename);

private:
    std::shared_ptr<dae::Texture2D> m_Texture{ nullptr };
    std::string m_Filename;
};
