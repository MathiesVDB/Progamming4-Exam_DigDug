#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Texture2D.h"
#include "GameObject.h"

using namespace dae;

TextureComponent::TextureComponent(dae::GameObject* owner, std::string filename)
	:   Component(owner),
		m_Filename(std::move(filename))
{
    m_Texture = dae::ResourceManager::GetInstance().LoadTexture(m_Filename);

    m_Width = m_Texture->GetSize().x;
    m_Height = m_Texture->GetSize().y;
}

void TextureComponent::Render() const
{
    auto owner = GetOwner();

    if (owner && m_Texture)
    {
	    if (auto transform = owner->GetComponent<dae::Transform>())
        {
            const auto& pos = transform->GetPosition();
            dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
        }
        else
        {
            std::cerr << "Error: Owner does not have a Transform component!\n";
        }
    }
}

void TextureComponent::Render(const float posX, const float posY, const SDL_Rect& sourceRect) const
{
	dae::Renderer::GetInstance().RenderTexture(*m_Texture, posX, posY, sourceRect);
}

void TextureComponent::SetTexture(const std::string& filename)
{
    if (m_Filename == filename && m_Texture) return; // Avoid redundant loading

    m_Filename = filename;
    m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);

	m_Width = m_Texture->GetSize().x;
	m_Height = m_Texture->GetSize().y;
}

int TextureComponent::GetWidth() const
{
	return m_Width;
}

int TextureComponent::GetHeight() const
{
	return m_Height;
}
