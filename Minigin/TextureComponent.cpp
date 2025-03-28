#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Texture2D.h"
#include "GameObject.h"

using namespace dae;

TextureComponent::TextureComponent(std::string filename)
    : m_Filename(std::move(filename))
{
    m_Texture = dae::ResourceManager::GetInstance().LoadTexture(m_Filename);
}

void TextureComponent::Render() const
{
    auto owner = GetOwner();

    if (owner && m_Texture)
    {
        auto transform = owner->GetComponent<dae::Transform>();

        if (transform)
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

void TextureComponent::SetTexture(const std::string& filename)
{
    if (m_Filename == filename && m_Texture) return; // Avoid redundant loading

    m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}
