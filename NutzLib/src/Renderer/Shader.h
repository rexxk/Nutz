#pragma once



namespace Nutz
{


	enum class ShaderDomain
	{
		Vertex,
		Fragment,
		Compute,
	};


	class Shader
	{
	public:
		static Ref<Shader> Create(const std::filesystem::path& filePath);

		virtual ~Shader() {}

		virtual void Shutdown() = 0;
		virtual void Reload() = 0;
	};


	class ShaderLibrary
	{
	public:
		static void Load(const std::filesystem::path& filePath);
		static void Shutdown();

		static bool IsEmpty();
	};
}
