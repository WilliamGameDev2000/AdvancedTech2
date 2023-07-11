#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;

class Object
{
	template<class T>
	friend class ObjectDrawable;
public:
	Object() = default;
	Object(const Object&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& gfx) const noexcept;
	void SetDrawable(bool);
	bool GetDrawable();
	virtual void Update(float dt) noexcept = 0;
	virtual void LoadTexture() noexcept = 0;
	virtual ~Object() = default;
protected:
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};
private:
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;

	bool is_drawable = true;
};