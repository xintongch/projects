import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppComponent } from './app.component';
import { NavbarComponent } from './components/navbar/navbar.component';
import { SearchPageComponent } from './components/search-page/search-page.component';
import { BookingsComponent } from './components/bookings/bookings.component';

import { HttpClientModule} from '@angular/common/http'; 
import { RouterModule, Routes } from '@angular/router';
import { NgbCarouselModule } from '@ng-bootstrap/ng-bootstrap';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';

import { CarouselModule } from 'ngx-owl-carousel-o';
import { GoogleMapsModule } from '@angular/google-maps';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations'

import { MatAutocompleteModule } from '@angular/material/autocomplete';
import { MatInputModule } from '@angular/material/input';
import { MatIconModule } from '@angular/material/icon';
import { MatButtonModule } from '@angular/material/button';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { AutoCompleteService } from './services/auto-complete.service';

import { ScrollingModule } from '@angular/cdk/scrolling';

import { Overlay, CloseScrollStrategy } from '@angular/cdk/overlay';


// import './polyfills';

import {CdkTableModule} from '@angular/cdk/table';
import {CdkTreeModule} from '@angular/cdk/tree';

// import { ScrollDispatcher } from '@angular/cdk/scrolling';

// import { ScrollDispatcher } from '@angular/cdk/scrolling';
// import {ScrollDispatchModule} from '@angular/cdk/scrolling';
// import {
//   MatBadgeModule,
//   MatBottomSheetModule,
//   MatButtonToggleModule,
//   MatCardModule,
//   MatCheckboxModule,
//   MatChipsModule,
//   MatDatepickerModule,
//   MatDialogModule,
//   MatDividerModule,
//   MatExpansionModule,
//   MatGridListModule,
//   MatListModule,
//   MatMenuModule,
//   MatNativeDateModule,
//   MatPaginatorModule,
//   MatProgressBarModule,
//   MatProgressSpinnerModule,
//   MatRadioModule,
//   MatRippleModule,
//   MatSelectModule,
//   MatSidenavModule,
//   MatSliderModule,
//   MatSlideToggleModule,
//   MatSnackBarModule,
//   MatSortModule,
//   MatStepperModule,
//   MatTableModule,
//   MatTabsModule,
//   MatToolbarModule,
//   MatTooltipModule,
//   MatTreeModule,
// } from '@angular/material';
import {platformBrowserDynamic} from '@angular/platform-browser-dynamic';
// import {AutocompleteOverviewExample} from './app/autocomplete-overview-example';



const appRoutes:Routes=[
  {path:'',component:SearchPageComponent,children:[
    {path:'',redirectTo:'search',pathMatch:'full'},
    // {path:'search',component:SearchPageComponent}
  ]},
  {path:'search',component:SearchPageComponent},
  {path:'bookings',component:BookingsComponent}
]

@NgModule({
  declarations: [
    AppComponent,
    NavbarComponent,
    SearchPageComponent,
    BookingsComponent,
    // CarouselModule
    // NgbdCarouselConfig,
  ],
  imports: [
    BrowserModule, 
    RouterModule.forRoot(appRoutes,{enableTracing:true}),
    HttpClientModule,
    CarouselModule,
    NgbCarouselModule,
    NgbModule,
    GoogleMapsModule,
    BrowserAnimationsModule,
    MatInputModule,
    MatIconModule,
    MatButtonModule,
    MatAutocompleteModule,
    FormsModule,
    ReactiveFormsModule,

    BrowserModule,
    BrowserAnimationsModule,
    FormsModule,
    HttpClientModule,
    // MatNativeDateModule,
    ReactiveFormsModule,

    ScrollingModule,
    // ScrollDispatcher
  ],
  providers: [AutoCompleteService,
    // { provide: MAT_AUTOCOMPLETE_SCROLL_STRATEGY, useFactory: scrollFactory, deps: [Overlay] }
  ],
  bootstrap: [AppComponent],
  exports:[
  //   ScrollDispatchModule,
  //   MatBadgeModule,
  // MatBottomSheetModule,
  // MatButtonToggleModule,
  // MatCardModule,
  // MatCheckboxModule,
  // MatChipsModule,
  // MatDatepickerModule,
  // MatDialogModule,
  // MatDividerModule,
  // MatExpansionModule,
  // MatGridListModule,
  // MatListModule,
  // MatMenuModule,
  // MatNativeDateModule,
  // MatPaginatorModule,
  // MatProgressBarModule,
  // MatProgressSpinnerModule,
  // MatRadioModule,
  // MatRippleModule,
  // MatSelectModule,
  // MatSidenavModule,
  // MatSliderModule,
  // MatSlideToggleModule,
  // MatSnackBarModule,
  // MatSortModule,
  // MatStepperModule,
  // MatTableModule,
  // MatTabsModule,
  // MatToolbarModule,
  // MatTooltipModule,
  // MatTreeModule,
  // ScrollDispatcher,
  // ScrollDispatcher
  ]
})
export class AppModule { }

export function scrollFactory(overlay: Overlay): () => CloseScrollStrategy {
  return () => overlay.scrollStrategies.close();
}

// platformBrowserDynamic().bootstrapModule(AppModule);
