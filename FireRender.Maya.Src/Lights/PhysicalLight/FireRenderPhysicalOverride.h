#pragma once

#include "PhysicalLightGeometryUtility.h"
#include <maya/MFnDependencyNode.h>
#include <maya/MNodeMessage.h>
#include <maya/MPxGeometryOverride.h>

#include <list>

/** The override is required for drawing to Maya's Viewport 2.0. */
class FireRenderPhysicalOverride : public MHWRender::MPxGeometryOverride
{
public:
	static MHWRender::MPxGeometryOverride* Creator(const MObject& obj)
	{
		return new FireRenderPhysicalOverride(obj);
	}

	virtual ~FireRenderPhysicalOverride();

	virtual MHWRender::DrawAPI supportedDrawAPIs() const override;

	virtual bool hasUIDrawables() const override;
	virtual void addUIDrawables(const MDagPath &path, MHWRender::MUIDrawManager &drawManager, const MHWRender::MFrameContext &frameContext) override;

	virtual void updateDG() override;
	virtual bool isIndexingDirty(const MHWRender::MRenderItem &item) override { return m_changed; }
	virtual bool isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc) override  { return m_changed; }
	virtual void updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList& list) override;
	virtual void populateGeometry(const MHWRender::MGeometryRequirements &requirements, const MHWRender::MRenderItemList &renderItems, MHWRender::MGeometry &data) override;
	virtual void cleanUp() override {};

	virtual bool refineSelectionPath(const MHWRender::MSelectionInfo &  	selectInfo,
		const MHWRender::MRenderItem &  	hitItem,
		MDagPath &  	path,
		MObject &  	components,
		MSelectionMask &  	objectMask
	) override;

	virtual void updateSelectionGranularity(
		const MDagPath& path,
		MHWRender::MSelectionContext& selectionContext) override;

private:
	FireRenderPhysicalOverride(const MObject& obj);

	void CreateGizmoGeometry(GizmoVertexVector& vertexVector, IndexVector& indexVector, const MDagPath& dagPath);
	void FillWithWireframeForMesh(GizmoVertexVector& vertexVector, IndexVector& indexVector, const MDagPath& dagPath);
	MColor GetColor(const MDagPath& path);

	void SubscribeSelectionChangedEvent(bool subscribe = true);

	static void onSelectionChanged(void *clientData);
	static void onImportantAttributeChanged(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void *clientData);

	void MakeSelectedMeshAsLight();

	bool IsPointLight() const;
	bool IsAreaMeshLight() const;
	bool HasNoGeomOverride() const;

private:
	MFnDependencyNode m_depNodeObj;
	MCallbackId m_attributeChangedCallback;
	MCallbackId m_selectionChangedCallback;

	bool m_changed;

	static MColor m_SelectedColor;
	static MColor m_Color;
};
