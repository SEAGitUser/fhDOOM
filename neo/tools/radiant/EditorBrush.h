/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free SoftwareF oundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

// brush.h

struct editorModel_t {
  idRenderModel* model;
  idBounds bounds;
  bool drawBounds;
};

struct entity_t;

struct brush_t {
	brush_t	*prev, *next;	// links in active/selected
	brush_t	*oprev, *onext;	// links in entity
	brush_t *   list;				//keep a handy link to the list its in
	entity_t	*owner;
	idVec3 mins, maxs;

	idVec3	lightCenter;			// for moving the shading center of point lights
	idVec3	lightRight;
	idVec3	lightTarget;
	idVec3	lightUp;
	idVec3	lightRadius;
	idVec3	lightOffset;
	idVec3	lightColor;
	idVec3	lightStart;
	idVec3	lightEnd;
	bool	pointLight;
	bool	startEnd;
	int		lightTexture;

	bool	trackLightOrigin;	// this brush is a special case light brush
	bool	entityModel;

	face_t  *brush_faces;

	//
	// curve brush extensions
	// all are derived from brush_faces
	bool	hiddenBrush;
	bool	forceWireFrame;
	bool	forceVisibile;

	patchMesh_t *pPatch;
	entity_t *pUndoOwner;

	int undoId;						//undo ID
	int redoId;						//redo ID
	int ownerId;					//entityId of the owner entity for undo

	int numberId;         // brush number

	idRenderModel	*modelHandle;
	mutable idRenderModel *animSnapshotModel;

	// brush primitive only
	idDict	epairs;
};

brush_t *	Brush_Alloc();
void		Brush_Free (brush_t *b, bool bRemoveNode = true);
int			Brush_MemorySize(const brush_t *b);
void		Brush_MakeSided (int sides);
void		Brush_MakeSidedCone (int sides);
void		Brush_Move (brush_t *b, const idVec3 move, bool bSnap = true, bool updateOrigin = true);
int			Brush_MoveVertex(brush_t *b, const idVec3 &vertex, const idVec3 &delta, idVec3 &end, bool bSnap);
void		Brush_ResetFaceOriginals(brush_t *b);
brush_t *	Brush_Parse (const idVec3 origin);
face_t *	Brush_Ray (idVec3 origin, idVec3 dir, brush_t *b, float *dist, bool testPrimitive = false);
void		Brush_RemoveFromList (brush_t *b);
void		Brush_AddToList (brush_t *b, brush_t *list);
void		Brush_Build(brush_t *b, bool bSnap = true, bool bMarkMap = true, bool bConvert = false, bool updateLights = true);
void		Brush_BuildWindings( brush_t *b, bool bSnap = true, bool keepOnPlaneWinding = false, bool updateLights = true, bool makeFacePlanes = true );
brush_t *	Brush_Clone (brush_t *b);
brush_t *	Brush_FullClone(brush_t *b);
brush_t *	Brush_Create (idVec3 mins, idVec3 maxs, texdef_t *texdef);
void		Brush_Draw( const brush_t *b, bool bSelected);
void		Brush_DrawXY(brush_t *b, ViewType nViewType, bool bSelected, const idVec3& color);
void		Brush_SplitBrushByFace (brush_t *in, face_t *f, brush_t **front, brush_t **back);
void		Brush_SelectFaceForDragging (brush_t *b, face_t *f, bool shear);
void		Brush_SetTexture (brush_t *b, texdef_t *texdef, brushprimit_texdef_t *brushprimit_texdef, bool bFitScale = false);
void		Brush_SideSelect (brush_t *b, idVec3 origin, idVec3 dir, bool shear);
void		Brush_SnapToGrid(brush_t *pb);
void		Brush_Rotate(brush_t *b, idVec3 vAngle, idVec3 vOrigin, bool bBuild = true);
void		Brush_MakeSidedSphere(int sides);
void		Brush_Write (brush_t *b, FILE *f, const idVec3 &origin, bool newFormat);
void		Brush_Write (brush_t *b, CMemFile* pMemFile, const idVec3 &origin, bool NewFormat);
idWinding *	Brush_MakeFaceWinding (brush_t *b, face_t *face, bool keepOnPlaneWinding = false);
void		Brush_SetTextureName(brush_t *b, const char *name);
void		Brush_Print(brush_t* b);
void		Brush_FitTexture( brush_t *b, float height, float width );
void		Brush_SetEpair(brush_t *b, const char *pKey, const char *pValue);
const char *Brush_GetKeyValue(brush_t *b, const char *pKey);
void		Brush_RebuildBrush(brush_t *b, idVec3 vMins, idVec3 vMaxs, bool patch = true);
void		Brush_GetBounds( brush_t *b, idBounds &bo );
editorModel_t Brush_GetEditorModel(const brush_t* b);

face_t *	Face_Alloc( void );
void		Face_Free( face_t *f );
face_t *	Face_Clone (face_t *f);
void		Face_MakePlane (face_t *f);

class fhImmediateMode;
void		Face_Draw( const face_t *face, const idVec4& color );
void		Face_DrawOutline( const face_t *face, const idVec3& color );
void		Brush_AddBrushLines(fhImmediateMode& im, const brush_t* brush, const brush_t* end, ViewType viewType);

void		Face_TextureVectors (face_t *f, float STfromXYZ[2][4]);
void		Face_FitTexture( face_t * face, float height, float width );
void		SetFaceTexdef (brush_t *b, face_t *f, texdef_t *texdef, brushprimit_texdef_t *brushprimit_texdef, bool bFitScale = false);

int			AddPlanept (idVec3 *f);
