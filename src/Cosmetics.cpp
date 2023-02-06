//
// Created by Rosier on 2023-01-21.
//

#include "Cosmetics.h"

bool Cosmetic::isTowerCosmetic()
{
    return (m_section == CosmeticSection::TowerCosmeticSkin || m_section == CosmeticSection::TowerCosmeticAsset ||
            m_section == CosmeticSection::TowerCosmeticProp || m_section == CosmeticSection::TowerCosmeticPet);
}
