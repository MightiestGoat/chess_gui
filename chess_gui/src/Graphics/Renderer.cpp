#include "Renderer.h"
#include "ECS/ECS.h"
#include <exception>
#include <stdexcept>

void Renderer::SetResourceBank(ResourceBank* resourceBank)
{
	this->resourceBank = resourceBank;
}

void Renderer::Bind(GBuffer& buffer)
{
	if(buffer.bindNo >= binders.size() || binders[buffer.bindNo].get() == nullptr) throw std::out_of_range("Bind No out of range");
	binders[buffer.bindNo]->Bind();
}
void Renderer::UnBind(GBuffer& buffer)
{
	if(buffer.bindNo >= binders.size() || binders[buffer.bindNo].get() == nullptr) throw std::out_of_range("Bind No out of range");
	binders[buffer.bindNo]->UnBind();
}
