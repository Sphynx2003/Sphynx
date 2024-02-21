#include "pch.h"
#include "GLMesh.h"
#include "glad/glad.h"
// #include "GLFW/glfw3.h"

void SetVertexAttribs(Sphynx::Core::Graphics::VertexBuffer* buffs, unsigned int& Index) {
	buffs->Bind();
	//TODO: Check.
	for (auto& e : buffs->GetLayout()) {
		switch (e.GetDataType())
		{
		case Sphynx::Core::Graphics::ShaderDataType::None:
			Core_Error("Attribute/Element Ignored.");
			//IDK, Error Handling ?
			Index--;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Float: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Float2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Float3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Float4:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_FLOAT, e.IsNormalized(),
				buffs->GetLayout().GetStride(), (void*)e.GetOffset());
			//Incrementing The index For the attribute
			Index++;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Double: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Double2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Double3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Double4:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_DOUBLE, e.IsNormalized(),
				buffs->GetLayout().GetStride(), (void*)e.GetOffset());
			//Incrementing The index For the attribute
			Index++;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Int: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Int2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Int3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Int4:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_INT, e.IsNormalized(),
				e.GetComponentCount() * sizeof(int), (void*)e.GetOffset());
			//Incrementing The index For the attribute
			Index++;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::UInt: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::UInt2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::UInt3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::UInt4:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_UNSIGNED_INT, e.IsNormalized(),
				buffs->GetLayout().GetStride(), (void*)e.GetOffset());
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Bool:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_FLOAT, e.IsNormalized(),
				buffs->GetLayout().GetStride(), (const void*)e.GetOffset());
			//Incrementing The index For the attribute
			Index++;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Mat2x2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat2x3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat2x4: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat3x2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat3x3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat3x4: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat4x2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat4x3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat4x4:
			for (unsigned int i = 0; i < e.GetComponentCount(); i++) {
				glEnableVertexAttribArray(Index);
				glVertexAttribPointer(Index, e.GetComponentCount(), GL_FLOAT, e.IsNormalized(),
					buffs->GetLayout().GetStride(), (const void*)e.GetOffset());
				//modify the rate at which generic vertex attributes advance during instanced rendering.
				glVertexAttribDivisor(Index, 1);
				//Incrementing The index For the attribute
				Index++;
			}
			break;
		default:
			Core_Error("Unknown Shader Data Type : {0},{1},{2}", __FILE__, __FUNCTION__, __LINE__);
			break;
		}
	}
	buffs->Unbind();
}

GLenum MeshTypeToGLenum(Sphynx::Core::Graphics::MeshType meshtype) {
	switch (meshtype)
	{
	case Sphynx::Core::Graphics::MeshType::Dynamic:
		return GL_DYNAMIC_DRAW;
	case Sphynx::Core::Graphics::MeshType::Static:
		return GL_STATIC_DRAW;
	case Sphynx::Core::Graphics::MeshType::Stream:
		return GL_STREAM_DRAW;
	default:
		return GL_NONE;
	}
}

Sphynx::Core::Graphics::GL::GLMesh::GLMesh(GLMesh&& Mesh)noexcept
{
	std::swap(this->VAO, Mesh.VAO);
	std::swap(this->VAttribIndex, Mesh.VAttribIndex);
	std::swap(this->VBuffers, Mesh.VBuffers);
	std::swap(this->IBuffer, Mesh.IBuffer);
	std::swap(this->hasIndexArray, Mesh.hasIndexArray);
}

Sphynx::Core::Graphics::GL::GLMesh& Sphynx::Core::Graphics::GL::GLMesh::operator=(GLMesh&& Mesh)noexcept
{
	if (this != &Mesh) {
		Release();
		std::swap(this->VAO, Mesh.VAO);
		std::swap(this->VAttribIndex, Mesh.VAttribIndex);
		std::swap(this->VBuffers, Mesh.VBuffers);
		std::swap(this->IBuffer, Mesh.IBuffer);
		std::swap(this->hasIndexArray, Mesh.hasIndexArray);
	}
	return *this;
}

Sphynx::Core::Graphics::GL::GLMesh::~GLMesh()
{
	Release();
}

Sphynx::Core::Graphics::GL::GLMesh::GLMesh()
{
	glCreateVertexArrays(1, &VAO);
}

Sphynx::Core::Graphics::GL::GLMesh::GLMesh(float* vertexes, size_t vertsize, unsigned int* indexes, size_t indexsize, MeshType meshtype)
{
	//Create VAO
	glCreateVertexArrays(1, &VAO);
	//Binding the Vertex Buffers in the VAO
	glBindVertexArray(VAO);
	//Create Vertex Buffer
	auto temp = new GLVertexBuffer(vertexes, vertsize);
	temp->SetDataLayout(BufferLayout({ BufferElement(ShaderDataType::Float3, false) }));
	VBuffers.push_back(temp);
	VBuffers[0]->Bind();
	VBuffers[0]->SetData(vertexes, 0, vertsize);
	SetVertexAttribs(temp, VAttribIndex);
	//Create and Bind Index Buffers
	if ((hasIndexArray = (indexes != nullptr))) {
		IBuffer = new GLIndexBuffer(indexes, indexsize / sizeof(*indexes));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(GLuint*)(IBuffer->GetNative()));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexsize, indexes, MeshTypeToGLenum(meshtype));
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	temp = nullptr;
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Sphynx::Core::Graphics::GL::GLMesh::GLMesh(VertexBuffer* VertBuf, IndexBuffer* IndexBuf) : VBuffers(), IBuffer((GLIndexBuffer*)IndexBuf)
{
	VBuffers.push_back(VertBuf);
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	VertBuf->Bind();
	if ((hasIndexArray = (IndexBuf != nullptr))) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(GLuint*)(IndexBuf->GetNative()));
	}
	//Attributes For Vertex Buffers
	SetVertexAttribs(VertBuf, VAttribIndex);
	//Unbinding The VAO for Clean-ness
	glBindVertexArray(0);
}

Sphynx::Core::Graphics::GL::GLMesh::GLMesh(std::vector<VertexBuffer*> VertBuf, IndexBuffer* IndexBuf)
	: VBuffers(VertBuf), IBuffer((GLIndexBuffer*)(IndexBuf))
{
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	if ((hasIndexArray = (IndexBuf != nullptr))) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(GLuint*)(IndexBuf->GetNative()));
	}
	//Attributes For Vertex Buffers
	for (auto* buffs : VertBuf)
	{
		//Attributes
		SetVertexAttribs(buffs, VAttribIndex);
	}
	//Unbinding The VAO for Clean-ness
	glBindVertexArray(0);
}

void Sphynx::Core::Graphics::GL::GLMesh::Bind()const
{
	glBindVertexArray(VAO);
}

void Sphynx::Core::Graphics::GL::GLMesh::Unbind()const
{
	glBindVertexArray(0);
}

void Sphynx::Core::Graphics::GL::GLMesh::AddVertexBuffer(VertexBuffer* VBuffer)
{
	Bind();
	VBuffers.push_back(VBuffer);
	SetVertexAttribs(VBuffer, VAttribIndex);
	Unbind();
}

void Sphynx::Core::Graphics::GL::GLMesh::AddVertexBuffers(std::vector<VertexBuffer*> _VBuffers)
{
	Bind();
	VBuffers.insert(VBuffers.end(), _VBuffers.begin(), _VBuffers.end());
	for (auto& vb : _VBuffers) {
		SetVertexAttribs(vb, VAttribIndex);
	}
	Unbind();
}

void Sphynx::Core::Graphics::GL::GLMesh::SetIndexBuffer(IndexBuffer* ibuf)
{
	if (ibuf == nullptr)return;
	hasIndexArray = true;
	IBuffer = static_cast<GLIndexBuffer*>(ibuf);
}

void Sphynx::Core::Graphics::GL::GLMesh::Release()
{
	if (this->hasIndexArray) {
		this->Unbind();
		IBuffer->Release();
	}
	glDeleteVertexArrays(1, &VAO);
	VAO = 0;
	for (auto v : VBuffers) { delete v; }
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::Release()
{
	Release();
	Size = 0;
	Data = NULL;
}

Sphynx::Core::Graphics::GL::GLVertexBuffer::GLVertexBuffer(size_t Size) : Size(Size),
	underlaying(Size, NULL, UsageHint::Static, AccessHint::Draw)
{
}

Sphynx::Core::Graphics::GL::GLVertexBuffer::GLVertexBuffer(float* verts, size_t Size) : Size(Size), 
	underlaying(Size, verts, UsageHint::Static, AccessHint::Draw)
{
	Data = verts;
}

Sphynx::Core::Graphics::GL::GLVertexBuffer::GLVertexBuffer(GLVertexBuffer&& vbuf) noexcept
{
	std::swap(this->underlaying, vbuf.underlaying);
	std::swap(this->Layout, vbuf.Layout);
	std::swap(this->Size, vbuf.Size);
	std::swap(this->Data, vbuf.Data);
}

Sphynx::Core::Graphics::GL::GLVertexBuffer& Sphynx::Core::Graphics::GL::GLVertexBuffer::operator=(GLVertexBuffer&& vbuf) noexcept
{
	if (this != &vbuf) {
		Release();
		std::swap(this->underlaying, vbuf.underlaying);
		std::swap(this->Layout, vbuf.Layout);
		std::swap(this->Size, vbuf.Size);
		std::swap(this->Data, vbuf.Data);
	}
	return *this;
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::Bind()const noexcept
{
	underlaying.Bind();
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::Unbind()const noexcept
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::SetData(const void* data, size_t size, size_t offset)noexcept
{
	Data = data;
	underlaying.SetData(Data, size, offset);
}

void* Sphynx::Core::Graphics::GL::GLVertexBuffer::Map(const Sphynx::Core::Graphics::MapAccess access)
{
	return underlaying.Map(access);
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::Unmap()noexcept
{
	underlaying.Unmap();
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::Reallocate(size_t size, void* data)
{
	underlaying.Reallocate(size, data);
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::Invalidate() noexcept
{
	underlaying.Invalidate();
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::Release()
{
	underlaying.Release();
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::Reallocate(size_t size, void* data)
{
	underlaying.Reallocate(size, data);
}

Sphynx::Core::Graphics::GL::GLIndexBuffer::GLIndexBuffer(uint32_t count)noexcept : Count(count), Data(NULL),
	underlaying(sizeof(int)* count, NULL, UsageHint::Dynamic, AccessHint::Draw)
{
	//glCreateBuffers(1, &BufferID);
	//// GL_ELEMENT_ARRAY_BUFFER need a bound VAO to be valid.
	//// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	//glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	//glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), 0, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Sphynx::Core::Graphics::GL::GLIndexBuffer::GLIndexBuffer(unsigned int* indices, size_t count)noexcept : Count(count),
	underlaying(sizeof(int)* count, indices, UsageHint::Dynamic, AccessHint::Draw)
{
	Data = (void*)indices;
	//glCreateBuffers(1, &BufferID);
	//// GL_ELEMENT_ARRAY_BUFFER need a bound VAO to be valid.
	//// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	//glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	//glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Sphynx::Core::Graphics::GL::GLIndexBuffer::GLIndexBuffer(GLIndexBuffer&& ibuf) noexcept
{
	std::swap(this->underlaying, ibuf.underlaying);
	std::swap(this->Count, ibuf.Count);
	std::swap(this->Data, ibuf.Data);
}

Sphynx::Core::Graphics::GL::GLIndexBuffer& Sphynx::Core::Graphics::GL::GLIndexBuffer::operator=(GLIndexBuffer&& ibuf) noexcept
{
	if (this != &ibuf) {
		Release();
		std::swap(this->underlaying, ibuf.underlaying);
		std::swap(this->Count, ibuf.Count);
		std::swap(this->Data, ibuf.Data);
	}
	return *this;
}

Sphynx::Core::Graphics::GL::GLIndexBuffer::~GLIndexBuffer()
{
	Release();
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::Bind() const noexcept
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(UINT*)underlaying.GetNative());
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::Unbind() const noexcept
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::SetData(const void* data, size_t size, size_t offset)
{
	underlaying.SetData(data, size, offset);
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::SetData(const unsigned int* data, uint64_t count)
{
	Data = data;
	underlaying.SetData(data, count * sizeof(unsigned int), 0);
	/*Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
	Unbind();*/
}

const void* Sphynx::Core::Graphics::GL::GLIndexBuffer::GetData()const noexcept
{
	return Data;
}

void* Sphynx::Core::Graphics::GL::GLIndexBuffer::Map(const MapAccess access)
{
	return underlaying.Map(access);
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::Unmap()noexcept
{
	underlaying.Unmap();
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::Invalidate() noexcept
{
	underlaying.Invalidate();
}
