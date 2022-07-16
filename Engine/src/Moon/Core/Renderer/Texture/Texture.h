#pragma once


namespace std::filesystem { class path; }

namespace Moon {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const RendererID& GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;

	};

	enum class Texture2DFilter
	{
		None = 0,
		Linear,
		Nearest,
		LinearMipmapLinear,
		LinearMipmapNearest,
		NearestMipmapLinear,
		NearestMipmapNearest,
	};

	enum class Texture2DWrap
	{
		None = 0,
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
	};

	struct Texture2DSpecification
	{
		Texture2DFilter MinFilter = Texture2DFilter::Linear;
		Texture2DFilter MagFilter = Texture2DFilter::Linear;

		Texture2DWrap WrapS = Texture2DWrap::Repeat;
		Texture2DWrap WrapT = Texture2DWrap::Repeat;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, const Texture2DSpecification& spec = Texture2DSpecification());
		static Ref<Texture2D> Create(const std::filesystem::path& filepath, const Texture2DSpecification& spec = Texture2DSpecification());

		virtual Texture2DSpecification GetSpecification() = 0;
		virtual void SetSpecification(const Texture2DSpecification& spec) = 0;

		virtual const std::filesystem::path& GetPath() const = 0;

	};

}
