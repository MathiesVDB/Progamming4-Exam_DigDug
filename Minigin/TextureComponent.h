#pragma once
#include <memory>

#include "Component.h"
#include <string>
#include <SDL.h>

#include "Texture2D.h"

class GameObject;

class TextureComponent final : public dae::Component
{
public:
    TextureComponent(dae::GameObject* owner, std::string  filename);
    TextureComponent(const TextureComponent&) = delete;
    TextureComponent(TextureComponent&&) = delete;
    TextureComponent& operator= (const TextureComponent&) = delete;
    TextureComponent& operator= (const TextureComponent&&) = delete;

    void Render() const override;
	void Render(const float posX, const float posY, const SDL_Rect& sourceRect) const;

    void SetTexture(const std::string& filename);

	int GetWidth()  const;
    int GetHeight() const;

private:
    std::shared_ptr<dae::Texture2D> m_Texture{ nullptr };
    std::string m_Filename;

	int m_Width{};
	int m_Height{};
};
