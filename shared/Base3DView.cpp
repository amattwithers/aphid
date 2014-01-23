
#include <QtGui>
#include <QtOpenGL>

#include "Base3DView.h"
#include <ToolContext.h>
#include <SelectionArray.h>
#include <BaseBrush.h>
#include <BaseCamera.h>
#include <PerspectiveCamera.h>
#include <KdTreeDrawer.h>
#include <IntersectionContext.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

//! [0]
Base3DView::Base3DView(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_backgroundColor = QColor::fromCmykF(0.29, 0.29, 0.20, 0.0);
	m_orthoCamera = new BaseCamera;
	m_perspCamera = new PerspectiveCamera;
	fCamera = m_orthoCamera;
	m_drawer = new KdTreeDrawer;
	m_activeComponent = new SelectionArray;
	m_activeComponent->setComponentFilterType(PrimitiveFilter::TVertex);
	m_intersectCtx = new IntersectionContext;
	m_intersectCtx->setComponentFilterType(PrimitiveFilter::TVertex);
	m_brush = new BaseBrush;
	
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
	m_timer->start(30);
	setFocusPolicy(Qt::ClickFocus);
	m_isFocused = 0;
	m_interactContext = 0;
}
//! [0]

//! [1]
Base3DView::~Base3DView()
{
	delete fCamera;
	delete m_drawer;
	delete m_activeComponent;
	delete m_intersectCtx;
}
//! [1]

//! [2]
QSize Base3DView::minimumSizeHint() const
{
    return QSize(50, 50);
}
//! [2]

//! [3]
QSize Base3DView::sizeHint() const
//! [3] //! [4]
{
    return QSize(640, 480);
}

BaseCamera * Base3DView::getCamera() const
{
	return fCamera;
}

BaseCamera * Base3DView::perspCamera()
{
	return m_perspCamera;
}

BaseCamera * Base3DView::orthoCamera()
{
	return m_orthoCamera;
}
	
KdTreeDrawer * Base3DView::getDrawer() const
{
	return m_drawer;
}

SelectionArray * Base3DView::getActiveComponent() const
{
	return m_activeComponent;
}

IntersectionContext * Base3DView::getIntersectionContext() const
{
	return m_intersectCtx;
}

const Ray * Base3DView::getIncidentRay() const
{	
	return &m_incidentRay;
}

void Base3DView::initializeGL()
{
	qglClearColor(m_backgroundColor.dark());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LEQUAL);
	getDrawer()->initializeProfile();
}
//! [6]

//! [7]
void Base3DView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float m[16];
	getCamera()->getMatrix(m);
	glMultMatrixf(m);
	
	getDrawer()->m_markerProfile.apply();
	getDrawer()->coordsys(20.f);
	clientDraw();
	if(m_isFocused) {
		Vector3F corners[4];
		getCamera()->frameCorners(corners[0], corners[1], corners[2], corners[3]);
		glColor3f(0.f, 0.f, 1.f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);
		glEnd();
	}
	glFlush();
}
//! [7]

//! [8]
void Base3DView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
	m_perspCamera->setPortWidth(width);
	m_perspCamera->setPortHeight(height);
	m_orthoCamera->setPortWidth(width);
	m_orthoCamera->setPortHeight(height);
	if(getCamera()->isOrthographic())
		updateOrthoProjection();
	else
		updatePerspProjection();
}
//! [8]

//! [9]
void Base3DView::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
    if(event->modifiers() == Qt::AltModifier) 
        return;
    
    processSelection(event);
}
//! [9]

void Base3DView::mouseReleaseEvent(QMouseEvent *event)
{
    processDeselection(event);
}

//! [10]
void Base3DView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->modifiers() == Qt::AltModifier)
        processCamera(event);
    else 
        processMouseInput(event);

    m_lastPos = event->pos();
}
//! [10]

void Base3DView::processCamera(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();
    if (event->buttons() & Qt::LeftButton) {
        getCamera()->tumble(dx, dy);
    } 
	else if (event->buttons() & Qt::MidButton) {
		getCamera()->track(dx, dy);
    }
	else if (event->buttons() & Qt::RightButton) {
		getCamera()->zoom(-dx / 2 + -dy / 2);
		if(getCamera()->isOrthographic())
			updateOrthoProjection();
		else
			updatePerspProjection();
    }
}

void Base3DView::processSelection(QMouseEvent *event)
{
	computeIncidentRay(event->x(), event->y());
	
    if(event->modifiers() == Qt::ShiftModifier) 
		m_activeComponent->disableVertexPath();
	else 
		m_activeComponent->enableVertexPath();
	clientSelect(event);
}

void Base3DView::processDeselection(QMouseEvent *event)
{
    clientDeselect(event);
}

void Base3DView::processMouseInput(QMouseEvent *event)
{
    computeIncidentRay(event->x(), event->y());
	
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();
    Vector3F injv;
    getCamera()->screenToWorldVector(dx, dy, injv);
    clientMouseInput(event);
}

void Base3DView::clientDraw()
{
    
}

void Base3DView::clientSelect(QMouseEvent *)
{
    
}

void Base3DView::clientDeselect(QMouseEvent *)
{
    
}

void Base3DView::clientMouseInput(QMouseEvent *)
{
    
}

Vector3F Base3DView::sceneCenter() const
{
    return Vector3F(0.f, 0.f, 0.f);
}

void Base3DView::updateOrthoProjection()
{
	makeCurrent();
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	float aspect = getCamera()->aspectRatio();
	float fov = getCamera()->fieldOfView();
	float right = fov/ 2.f;
	float top = right / aspect;

    glOrtho(-right, right, -top, top, getCamera()->nearClipPlane(), getCamera()->farClipPlane());

    glMatrixMode(GL_MODELVIEW);
	doneCurrent();
}

void Base3DView::updatePerspProjection()
{
	makeCurrent();
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	GLdouble left,right,bottom,top;
		
	right = getCamera()->frameWidth() * 0.5f;
	left = -right;
	top = getCamera()->frameHeight() * 0.5f;
	bottom = -top;

    glFrustum(left, right, bottom, top, getCamera()->nearClipPlane(), getCamera()->farClipPlane());
	
    glMatrixMode(GL_MODELVIEW);
	doneCurrent();
}

void Base3DView::resetView()
{
	getCamera()->reset(sceneCenter());
	if(getCamera()->isOrthographic())
		updateOrthoProjection();
	else
		updatePerspProjection();
}

void Base3DView::drawSelection()
{
	m_drawer->setColor(0.f, .8f, .2f);
	m_drawer->components(m_activeComponent);
}

void Base3DView::clearSelection()
{
	m_activeComponent->reset();
}

void Base3DView::addHitToSelection()
{
	m_activeComponent->add(m_intersectCtx->m_geometry, m_intersectCtx->m_componentIdx, m_intersectCtx->m_hitP);
}

void Base3DView::growSelection()
{
	m_activeComponent->grow();
}

void Base3DView::shrinkSelection()
{
	m_activeComponent->shrink();
}

void Base3DView::frameAll()
{
    Vector3F coi = sceneCenter();
    Vector3F eye = coi + Vector3F(0.f, 0.f, 100.f);
    
    getCamera()->lookFromTo(eye, coi);
}

void Base3DView::keyPressEvent(QKeyEvent *e)
{
	if(e->key() == Qt::Key_O) {
		if(getCamera()->isOrthographic()) {
			fCamera = m_perspCamera;
			fCamera->copyTransformFrom(m_orthoCamera);
			updatePerspProjection();
		}
		else {
			fCamera = m_orthoCamera;
			fCamera->copyTransformFrom(m_perspCamera);
			updateOrthoProjection();
		}
	}
	
	switch (e->key()) {
		case Qt::Key_Space:
			clearSelection();
			break;
		case Qt::Key_H:
			resetView();
			break;
		case Qt::Key_BracketRight:
			growSelection();
			break;
		case Qt::Key_BracketLeft:
			shrinkSelection();
			break;
		case Qt::Key_Up:
			getCamera()->moveForward(23);
			break;
		case Qt::Key_Down:
			getCamera()->moveForward(-23);
			break;
		case Qt::Key_G:
			frameAll();
			break;
		default:
			break;
	}
	
	QWidget::keyPressEvent(e);
}

void Base3DView::focusInEvent(QFocusEvent * event)
{
	m_isFocused = 1;
	m_timer->start(30);
	QGLWidget::focusInEvent(event);
}

void Base3DView::focusOutEvent(QFocusEvent * event)
{
	m_isFocused = 0;
	m_timer->stop();
	QGLWidget::focusOutEvent(event);
}

void Base3DView::drawIntersection() const
{
    IntersectionContext * ctx = getIntersectionContext();
    if(!ctx->m_success) return;
    
    getDrawer()->circleAt(ctx->m_hitP, ctx->m_hitN);
	getDrawer()->boundingBox(ctx->m_bbox);
    getDrawer()->arrow(ctx->m_hitP, ctx->m_hitP + ctx->m_hitN);
}

const BaseBrush * Base3DView::brush() const
{
	return m_brush;
}

BaseBrush * Base3DView::brush()
{
	return m_brush;
}

void Base3DView::showBrush() const
{
	getDrawer()->colorAsReference();
	getDrawer()->circleAt(brush()->getSpace(), brush()->getRadius());

    if(brush()->length() > 10e-3)
        getDrawer()->arrow(brush()->heelPosition(), brush()->toePosition());
}

void Base3DView::computeIncidentRay(int x, int y)
{
	Vector3F origin, direct;
    getCamera()->incidentRay(x, y, origin, direct);
    direct.normalize();
	m_incidentRay = Ray(origin + direct * getCamera()->nearClipPlane(), origin + direct * getCamera()->farClipPlane());
}


void Base3DView::receiveBrushRadius(double x)
{
    brush()->setRadius(x);
	update();
}

void Base3DView::receiveBrushPitch(double x)
{
    brush()->setPitch(x);
	update();
}

void Base3DView::receiveBrushNumSamples(int x)
{
    brush()->setNumDarts(x);
	update();
}

void Base3DView::receiveBrushStrength(double x)
{
	brush()->setStrength(x);
}

void Base3DView::receiveBrushTwoSided(int x)
{
	if(x == Qt::Checked) brush()->setTwoSided(true);
	else brush()->setTwoSided(false);
}

void Base3DView::receiveBrushFilterByColor(int x)
{
	if(x == Qt::Checked) brush()->setFilterByColor(true);
	else brush()->setFilterByColor(false);
}

QPoint Base3DView::lastMousePos() const
{
	return m_lastPos;
}

void Base3DView::setInteractContext(ToolContext * ctx)
{
	m_interactContext = ctx;
}

int Base3DView::interactMode() const
{
	if(!m_interactContext) return ToolContext::SelectVertex;
	
	return m_interactContext->getContext();
}

void Base3DView::usePerspCamera()
{
	if(getCamera()->isOrthographic()) {
		fCamera = m_perspCamera;
		fCamera->copyTransformFrom(m_orthoCamera);
		updatePerspProjection();
	}
		
}

void Base3DView::useOrthoCamera()
{
	if(!getCamera()->isOrthographic()) {
		fCamera = m_orthoCamera;
		fCamera->copyTransformFrom(m_perspCamera);
		updateOrthoProjection();
	}
}
//:~
