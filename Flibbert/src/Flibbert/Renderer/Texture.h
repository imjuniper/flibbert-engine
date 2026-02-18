#pragma once

namespace Flibbert {

class ITexture
{
public:
	virtual ~ITexture() = default;

	virtual void MakeResident() = 0;
	virtual void MakeNonResident() = 0;

	[[nodiscard]] uint64_t GetHandle() const {
		return m_BindlessHandle;
	}

	[[nodiscard]] uint32_t GetWidth() const
	{
		return m_Width;
	}
	[[nodiscard]] uint32_t GetHeight() const
	{
		return m_Height;
	}
	[[nodiscard]] bool IsLoaded() const
	{
		return m_Loaded;
	}

	static std::shared_ptr<ITexture> Create(std::string_view path);

protected:
	std::string m_Path;
	bool m_Loaded = false;

	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	uint32_t m_InternalFormat = 0;
	uint32_t m_DataFormat = 0;
	uint64_t m_BindlessHandle = 0;
};

} // namespace Flibbert
