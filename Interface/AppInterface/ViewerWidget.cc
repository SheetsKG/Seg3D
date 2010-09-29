/*
 For more information, please see: http://software.sci.utah.edu

 The MIT License

 Copyright (c) 2009 Scientific Computing and Imaging Institute,
 University of Utah.


 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */


// Core includes
#include <Core/Utils/Log.h>
#include <Core/Interface/Interface.h>
#include <Core/State/Actions/ActionAdd.h>
#include <Core/State/Actions/ActionFlip.h>
#include <Core/State/Actions/ActionSet.h>

// Application includes
#include <Application/Viewer/Actions/ActionAutoView.h>
#include <Application/ViewerManager/ViewerManager.h>

// QT includes
#include <QtGui>
#include <QtOpenGL>

// Qt Interface support classes
#include <QtUtils/Utils/QtApplication.h>
#include <QtUtils/Bridge/QtBridge.h>
#include <QtUtils/Utils/QtRenderResources.h>
#include <QtUtils/Utils/QtRenderWidget.h>

// Ui includes
#include "ui_ViewerWidget.h"

// Interface includes
#include <Interface/AppInterface/SingleShotToolButton.h>
#include <Interface/AppInterface/StyleSheet.h>
#include <Interface/AppInterface/ViewerWidget.h>

namespace Seg3D
{

class ViewerWidgetPrivate
{
public:
	void handle_widget_activated();

	Ui::ViewerWidget ui_;
	QtUtils::QtRenderWidget* render_widget_;

	int minimum_toolbar_width_;	
	bool initialized_size_;

	// We need a special single shot button for the picking button
	QToolButton* picking_button_;
	QIcon picking_icon_;

	// Handle to the underlying Viewer structure
	ViewerHandle viewer_;
	
	QVector< QToolButton* > buttons_;
};

void ViewerWidgetPrivate::handle_widget_activated()
{
	if ( QApplication::keyboardModifiers() == Qt::ControlModifier )
	{
		Core::ActionAdd::Dispatch( Core::Interface::GetWidgetActionContext(),
			ViewerManager::Instance()->active_viewer_state_, 
			static_cast< int >( this->viewer_->get_viewer_id() ) );
	}
	else
	{
		std::set< int > active_viewer;
		active_viewer.insert( static_cast< int >( this->viewer_->get_viewer_id() ) );
		Core::ActionSet::Dispatch( Core::Interface::GetWidgetActionContext(),
			ViewerManager::Instance()->active_viewer_state_, active_viewer );
	}
}

ViewerWidget::ViewerWidget( ViewerHandle viewer, QWidget *parent ) :
	QWidget( parent ),
	private_( new ViewerWidgetPrivate )
{
	this->private_->viewer_ = viewer;
	this->private_->ui_.setupUi( this );

	// Setup the Icon
	this->private_->picking_icon_.addPixmap( QPixmap( ":/Images/Picking.png" ), QIcon::Normal, QIcon::On );
	this->private_->picking_icon_.addPixmap( QPixmap( ":/Images/PickingOff.png" ), QIcon::Normal, QIcon::Off );
	
	// Setup the Custom Picking Button
	this->private_->picking_button_ =  new SingleShotToolButton( this->private_->ui_.less_common_tools_ );
	this->private_->picking_button_->setCheckable( true );
	this->private_->picking_button_->setToolButtonStyle( Qt::ToolButtonIconOnly );
	this->private_->picking_button_->setIcon( this->private_->picking_icon_ );
	this->private_->picking_button_->setText( QString( "Picking" ) );
	this->private_->picking_button_->setToolTip( QString( "Make this viewer a target for picking" ) );
	this->private_->picking_button_->setFixedHeight( 20 );
	this->private_->picking_button_->setFixedWidth( 20 );
	
	this->private_->picking_button_->setMinimumHeight( 20 );
	this->private_->picking_button_->setMinimumWidth( 20 );
	this->private_->picking_button_->setMaximumHeight( 20 );
	this->private_->picking_button_->setMaximumWidth( 20 );
	
	// IF YOU ADD ANOTHER BUTTON TO THE VIEWERWIDGET, PLEASE ADD IT TO THE buttons_ VECTOR.
	// We make a vector of all the buttons this way we can calculate the minimum size that the 
	// viewer bar can be
	this->private_->buttons_.push_back( this->private_->ui_.auto_view_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.lock_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.slice_visible_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.light_visible_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.grid_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.flip_horizontal_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.flip_vertical_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.isosurfaces_visible_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.slices_visible_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.volume_rendering_visible_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.overlay_visible_button_ );
	this->private_->buttons_.push_back( this->private_->ui_.picking_lines_visible_button_ );
	this->private_->buttons_.push_back( this->private_->picking_button_ );
	
	this->private_->ui_.less_common_tools_layout_->insertWidget( 0, this->private_->picking_button_ );
	
//	// Setup the widget so for a small size it can be broken into two
//
//	this->private_->minimum_toolbar_width_ = 300;
	this->private_->initialized_size_ = false;
	this->private_->ui_.buttonbar_->setMinimumSize( QSize( 170, 0 ) );
	this->private_->ui_.button_layout_->setStretchFactor( this->private_->ui_.common_tools_, 0 );
	this->private_->ui_.button_layout_->setStretchFactor( this->private_->ui_.less_common_tools_, 1 );
	
	// --------------------------------------
	// Generate the OpenGL part of the widget
	this->private_->render_widget_ = QtUtils::QtApplication::Instance()->
		qt_renderresources_context()->create_qt_render_widget( this, this->private_->viewer_ );
	
	if( this->private_->render_widget_ == 0 )
	{
		CORE_THROW_LOGICERROR("OpenGL was not initialized correctly");
	}
	
	this->private_->render_widget_->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	this->private_->ui_.viewer_layout_->addWidget( this->private_->render_widget_ );
	

	// Update state of the widget to reflect current state
	{
		Core::StateEngine::lock_type lock( Core::StateEngine::GetMutex() );
		
		ViewerWidgetHandle viewer_widget( this );
		
		this->add_connection( this->private_->viewer_->view_mode_state_->value_changed_signal_.connect( 
			boost::bind( &ViewerWidget::HandleViewModeChanged, viewer_widget ) ) );

		this->connect( this->private_->ui_.viewer_mode_,
			SIGNAL( currentIndexChanged( int ) ), SLOT( change_view_type( int ) ) );

		this->connect( this->private_->ui_.flip_horizontal_button_, SIGNAL( clicked() ),
			SLOT( flip_view_horiz() ) );

		this->connect( this->private_->ui_.flip_vertical_button_, SIGNAL( clicked() ),
			SLOT( flip_view_vert() ) );

		this->connect( this->private_->ui_.auto_view_button_, SIGNAL( clicked() ),
			SLOT( auto_view() ) );

	
		QtUtils::QtBridge::Connect( this->private_->ui_.viewer_mode_, 
			this->private_->viewer_->view_mode_state_ );
		QtUtils::QtBridge::Connect( this->private_->picking_button_ , 
			this->private_->viewer_->is_picking_target_state_ );
		QtUtils::QtBridge::Connect( this->private_->ui_.grid_button_, 
			this->private_->viewer_->slice_grid_state_ );
		QtUtils::QtBridge::Connect( this->private_->ui_.lock_button_, 
			this->private_->viewer_->lock_state_ );
		QtUtils::QtBridge::Connect( this->private_->ui_.slice_visible_button_,
			this->private_->viewer_->slice_visible_state_ );
		QtUtils::QtBridge::Connect( this->private_->ui_.slices_visible_button_,
			this->private_->viewer_->volume_slices_visible_state_ );
		QtUtils::QtBridge::Connect( this->private_->ui_.picking_lines_visible_button_,
			this->private_->viewer_->slice_picking_visible_state_ );
		QtUtils::QtBridge::Connect( this->private_->ui_.overlay_visible_button_,
			this->private_->viewer_->overlay_visible_state_ );
		QtUtils::QtBridge::Connect( this->private_->ui_.light_visible_button_,
			this->private_->viewer_->volume_light_visible_state_ );
		QtUtils::QtBridge::Connect( this->private_->ui_.isosurfaces_visible_button_,
			this->private_->viewer_->volume_isosurfaces_visible_state_ );
		QtUtils::QtBridge::Connect( this->private_->ui_.volume_rendering_visible_button_,
			this->private_->viewer_->volume_volume_rendering_visible_state_ );
		this->add_icons_to_combobox();
		
		this->private_->render_widget_->activate_signal_.connect(
			boost::bind( &ViewerWidgetPrivate::handle_widget_activated, this->private_  ) );
	}
}
	
void ViewerWidget::add_icons_to_combobox()
{	
	QIcon icon;
	icon.addFile( QString::fromUtf8(":/Images/Xview.png"), QSize(), QIcon::Normal, QIcon::On );
	this->private_->ui_.viewer_mode_->setItemIcon( 0, icon );
	
	QIcon icon1;
	icon1.addFile( QString::fromUtf8(":/Images/Yview.png"), QSize(), QIcon::Normal, QIcon::On );
	this->private_->ui_.viewer_mode_->setItemIcon( 1, icon1 );
	
	QIcon icon2;
	icon2.addFile( QString::fromUtf8(":/Images/Zview.png"), QSize(), QIcon::Normal, QIcon::On );
	this->private_->ui_.viewer_mode_->setItemIcon( 2, icon2 );
	
	QIcon icon3;
	icon3.addFile( QString::fromUtf8(":/Images/Vview.png"), QSize(), QIcon::Normal, QIcon::On );
	this->private_->ui_.viewer_mode_->setItemIcon( 3, icon3 );	
}

ViewerWidget::~ViewerWidget()
{
	this->disconnect_all();
}
	
int ViewerWidget::get_minimum_size()
{
	// We start with padding the minimum width by 1 because of the 1px margin on the left-hand side
	int minimum_width = 1;
	
	// Next we get the width of the viewer mode holder and we pad it by 2 for the left and right 
	// margins
	int viewer_mode_width = this->private_->ui_.viewer_mode_->width() + 2;
	
	// Now we combine them.
	minimum_width = minimum_width + viewer_mode_width;
	
	// Now we add the sizes of the visible buttons plus a 1px padding for the right hand margin
	for( int i = 0; i < this->private_->buttons_.size(); ++i )
	{	
		if( this->private_->buttons_[ i ]->isVisible() ) 
		{
			minimum_width = minimum_width + this->private_->buttons_[ i ]->minimumWidth() + 1;
		}
	}
	
	// Now we return the final value
	return minimum_width;
}

void ViewerWidget::resizeEvent( QResizeEvent * event )
{
	// In the case that the widget hasnt been initialized we just use the arbitrary minimum size of
	// 300px
	if( !this->private_->initialized_size_ )
	{
		this->private_->minimum_toolbar_width_ = 300;
	}
	else
	{
		this->private_->minimum_toolbar_width_ = this->get_minimum_size();
	}

	
	int old_width = event->oldSize().width();
	int new_width = event->size().width();
	
	if ( new_width <= this->private_->minimum_toolbar_width_ &&
		( old_width > this->private_->minimum_toolbar_width_ || 
		 ! this->private_->initialized_size_ ) )
	{
		this->private_->ui_.sep_line_->hide();		
		this->private_->ui_.button_layout_->removeWidget( this->private_->ui_.less_common_tools_ );
		this->private_->ui_.toolbar_layout_->addWidget( this->private_->ui_.less_common_tools_, 0 );
		
		this->update();
	}
	else if ( new_width > this->private_->minimum_toolbar_width_ &&
			 ( old_width <= this->private_->minimum_toolbar_width_ || 
			  ! this->private_->initialized_size_ ) )
	{
		this->private_->ui_.sep_line_->show();	
		this->private_->ui_.toolbar_layout_->removeWidget( this->private_->ui_.less_common_tools_ );
		this->private_->ui_.button_layout_->addWidget( this->private_->ui_.less_common_tools_, 1 );
		
		this->update();
	}
	
	this->private_->initialized_size_ = true;
	
	QWidget::resizeEvent( event );
}
	
void ViewerWidget::select()
{
	this->private_->ui_.border_->setStyleSheet( StyleSheet::VIEWERSELECTED_C );
}

void ViewerWidget::deselect()
{
	this->private_->ui_.border_->setStyleSheet( StyleSheet::VIEWERNOTSELECTED_C );
}

void ViewerWidget::change_view_type( int index )
{
	bool is_volume_view = ( index == 3 );

	// 2D viewer specific buttons
	this->private_->ui_.flip_horizontal_button_->setVisible( !is_volume_view );
	this->private_->ui_.flip_vertical_button_->setVisible( !is_volume_view );
	this->private_->ui_.grid_button_->setVisible( !is_volume_view );
	this->private_->ui_.slice_visible_button_->setVisible( !is_volume_view );
	this->private_->ui_.picking_lines_visible_button_->setVisible( !is_volume_view );
	this->private_->picking_button_->setVisible( !is_volume_view );

	// 3D viewer specific buttons
	this->private_->ui_.slices_visible_button_->setVisible( is_volume_view );
	this->private_->ui_.light_visible_button_->setVisible( is_volume_view );
	this->private_->ui_.isosurfaces_visible_button_->setVisible( is_volume_view );
	this->private_->ui_.volume_rendering_visible_button_->setVisible( is_volume_view);
	
	// disable buttons that we dont use yet.
	this->private_->ui_.volume_rendering_visible_button_->setEnabled( false );
	this->private_->ui_.overlay_visible_button_->setEnabled( !is_volume_view );
	
	this->resize( this->width(), this->height() );
}
	
void ViewerWidget::HandleViewModeChanged( ViewerWidgetHandle viewer_widget )
{
	if( !( Core::Interface::IsInterfaceThread() ) )
	{
		Core::Interface::Instance()->post_event( boost::bind( &ViewerWidget::HandleViewModeChanged,
			viewer_widget ) );
		return;
	}
	
	if( viewer_widget.data() )
	{
		Core::StateEngine::lock_type lock( Core::StateEngine::GetMutex() );
		bool is_volume_view = viewer_widget->private_->viewer_->is_volume_view();

		if( !is_volume_view )
		{
			Core::StateView2D* view2d_state = static_cast< Core::StateView2D* >( 
				viewer_widget->private_->viewer_->get_active_view_state().get() );
			viewer_widget->private_->ui_.flip_horizontal_button_->setChecked( view2d_state->x_flipped() );
			viewer_widget->private_->ui_.flip_vertical_button_->setChecked( view2d_state->y_flipped() );
		}
	}
}
	
void ViewerWidget::flip_view_horiz()
{
	if( ! this->private_->viewer_->is_volume_view() )
	{
		Core::StateView2DHandle view2d_state = boost::dynamic_pointer_cast<Core::StateView2D>( 
			this->private_->viewer_->get_active_view_state() );
		Core::ActionFlip::Dispatch( Core::Interface::GetWidgetActionContext(),
			view2d_state, Core::FlipDirectionType::HORIZONTAL_E );
	}
}

void ViewerWidget::flip_view_vert()
{
	if( ! this->private_->viewer_->is_volume_view() )
	{
		Core::StateView2DHandle view2d_state = boost::dynamic_pointer_cast<Core::StateView2D>( 
			this->private_->viewer_->get_active_view_state() );
		Core::ActionFlip::Dispatch( Core::Interface::GetWidgetActionContext(),
		view2d_state, Core::FlipDirectionType::VERTICAL_E );
	}
}

void ViewerWidget::auto_view()
{
	ActionAutoView::Dispatch( Core::Interface::GetWidgetActionContext(), this->private_->viewer_->get_viewer_id() );
}

} // end namespace Seg3D