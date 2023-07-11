#include "Object.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Object::Draw(Graphics& gfx) const noexcept
{
	if (is_drawable)
	{
		for (auto& b : binds)
		{
			b->Bind(gfx);
		}
		for (auto& b : GetStaticBinds())
		{
			b->Bind(gfx);
		}
		gfx.DrawIndexed(pIndexBuffer->GetCount());
	}
}

void Object::SetDrawable(bool can_draw)
{
	is_drawable = can_draw;
}

bool Object::GetDrawable()
{
	return is_drawable;
}

void Object::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Object::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}