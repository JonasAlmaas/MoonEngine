#pragma once


namespace Moon {
	
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
		
		operator const char* () const { return Tag.c_str(); }
	};

}
