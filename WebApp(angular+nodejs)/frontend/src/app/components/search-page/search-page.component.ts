import { getLocaleCurrencyCode } from '@angular/common';
import { Component, OnInit, ViewChild } from '@angular/core';
import { Router } from '@angular/router';

import { apidataService } from '../../services/api-data.service';
import { DisplayResultsService } from 'src/app/services/display-results.service';
import { DisplayDetailsService } from 'src/app/services/display-details.service';
import { AutoCompleteService } from 'src/app/services/auto-complete.service';
import { ClickSubmitSearchService } from 'src/app/services/click-submit-search.service';
import { ClickSubmitReservationService } from 'src/app/services/click-submit-reservation.service';
import { DisplayBusinessPhotosService } from 'src/app/services/display-business-photos.service';
import { ReservationDataService } from 'src/app/services/reservation-data.service';
import { DeleteBookingService } from 'src/app/services/delete-booking.service';

import * as $ from 'jquery';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { OwlOptions } from 'ngx-owl-carousel-o';

import { HostListener } from '@angular/core';

import { GoogleMap } from '@angular/google-maps';

import {
  FormBuilder,
  FormGroup,
  Validators,
  FormGroupDirective,
} from '@angular/forms';

import { ScrollingModule } from '@angular/cdk/scrolling';

import {
  debounceTime,
  tap,
  switchMap,
  finalize,
  distinctUntilChanged,
  filter,
} from 'rxjs/operators';
import { FormControl } from '@angular/forms';
import { HttpClient, HttpHeaders, HttpParams } from '@angular/common/http';

import { Directive, Input, OnDestroy } from '@angular/core';
import { MatAutocompleteTrigger } from '@angular/material/autocomplete';

@Component({
  selector: 'app-search-page',
  templateUrl: './search-page.component.html',
  styleUrls: ['./search-page.component.css'],
})
export class SearchPageComponent implements OnInit {
  private matAutocompleteTrigger: MatAutocompleteTrigger;
  @ViewChild('autoCompleteInput', { read: MatAutocompleteTrigger })
  autoComplete0: MatAutocompleteTrigger;
  reservationForm!: FormGroup;
  submitted = false;

  width = 790;
  height = 700;
  zoom = 14;
  center = {
    lat: 0,
    lng: 0,
  };
  marker = {
    position: { lat: 0, lng: 0 },
  };

  constructor(
    private router: Router,
    private apidataService: apidataService,
    private http: HttpClient,
    private displayResultsService: DisplayResultsService,
    private displayDetailsService: DisplayDetailsService,
    private autoCompleteService: AutoCompleteService,
    private clickSubmitSearchService: ClickSubmitSearchService,
    private clickSubmitReservationService: ClickSubmitReservationService,
    private displayBusinessPhotosService: DisplayBusinessPhotosService,
    private reservationDataService: ReservationDataService,
    private deleteBookingService: DeleteBookingService,
    private formBuilder: FormBuilder
  ) {}

  searchMoviesCtrl = new FormControl();
  filteredMovies: any;
  isLoading = false;
  errorMsg!: string;
  minLengthTerm = 1;
  selectedMovie: any = '';

  onSelected() {
    this.selectedMovie = this.selectedMovie;
  }

  displayWith(value: any) {
    return value?.Title;
  }

  autoComplete() {
    this.searchMoviesCtrl.valueChanges
      .pipe(
        filter((res: any) => {
          return res !== null && res.length >= this.minLengthTerm;
        }),
        distinctUntilChanged(),
        debounceTime(1000),
        tap(() => {
          this.errorMsg = '';
          this.filteredMovies = [];
          this.isLoading = true;
        }),
        switchMap((value) =>
          this.http.get('/api/autoComplete?text=' + value).pipe(
            finalize(() => {
              this.isLoading = false;
            })
          )
        )
      )
      .subscribe((data: any) => {
        let list: any = [];
        let index = 0;
        data.categories.forEach((ca: any) => {
          list[index] = {};
          list[index].Title = ca.title;
          index += 1;
        });
        data.terms.forEach((te: any) => {
          list[index] = {};
          list[index].Title = te.text;
          index += 1;
        });
        this.filteredMovies = list;
      });
  }

  SetMinDate() {
    var now = new Date();
    var day = ('0' + now.getDate()).slice(-2);
    var month = ('0' + (now.getMonth() + 1)).slice(-2);
    var today = now.getFullYear() + '-' + month + '-' + day;
    $('#date').attr('min', today);
  }

  ngOnInit(): void {
    console.log('search-page');
    (<HTMLInputElement>document.getElementById('detailCardNCradio1')).checked =
    true;
    // window.localStorage.clear();
    this.SetMinDate();
    this.reservationForm = this.formBuilder.group({
      Email: new FormControl('', [Validators.required, Validators.email]),
      Date: ['', Validators.required],
      Hour: ['', Validators.required],
      Minute: ['', Validators.required],
    });

    $('#header1').css({
      border: '2px black solid',
    });
    $('#header2').css({
      border: 'none',
    });

    $('#rt-container').attr('style', 'display:none');
    $('#dc-container').attr('style', 'display:none');
    $('#no-result').css({
      display: 'none',
    });
    this.autoComplete();
    (<HTMLInputElement>document.getElementById('search-form')).addEventListener(
      'submit',
      (e) => {
        e.preventDefault();
        this.clickSubmitSearchService.clickSubmit();
      }
    );

    let smallScreen = window.matchMedia('(max-width: 500px)');
    if (smallScreen.matches) {
      this.width = 360;
      this.height = 531;
      $('#bottom-line').css({
        left: '0',
      });
    }

    let largeScreen = window.matchMedia('(min-width: 500px)');
    if (largeScreen.matches) {
      this.width = 790;
      this.height = 496;
    }
  }

  clickClear(): void {
    $('#search-form input').val('');
    (<HTMLInputElement>document.getElementById('location')).disabled = false;
    $('#no-result').css({
      display: 'none',
    });
    $('#dc-container').css({
      display: 'none',
    });
    $('#rt-container').css({
      display: 'none',
    });
  }

  clickReserveNow() {
    this.submitted = false;
    this.reservationForm.reset();
  }

  autoDetectOnChange() {
    if ((<HTMLInputElement>document.getElementById('auto-detect')).checked) {
      (<HTMLInputElement>document.getElementById('location')).disabled = true;
      $('#location').val('');
    } else {
      (<HTMLInputElement>document.getElementById('location')).disabled = false;
    }
  }

  prev() {
    this.displayBusinessPhotosService.prev();
  }

  next() {
    this.displayBusinessPhotosService.next();
  }

  return_from_detail_card() {
    $('#dc-container').css({ display: 'none' });
    $('#rt-container').css({ display: 'inline-block' });
  }

  slideBack() {
    (<HTMLInputElement>document.getElementById('detailCardNCradio1')).checked =
      true;
    (<HTMLInputElement>(
      document.getElementById('detailCardNavigationCarouselbtn-radio1')
    )).checked = true;
    $('.prev-btn.detailCardNavigationCarouselbtn img').attr(
      'src',
      'assets/prev7.png'
    );
    $('.next-btn.detailCardNavigationCarouselbtn img').attr(
      'src',
      'assets/next5.png'
    );
    let smallScreen = window.matchMedia('(max-width: 500px)');
    if (smallScreen.matches) {
      if((<HTMLInputElement>(document.getElementById('slide-controllers-slide-radio1')
      )).checked == true){
        $('#bottom-line').css({
          width:'160px',
          left: '0px',
          transition:'0s'
        });
      }
      if((<HTMLInputElement>(document.getElementById('slide-controllers-slide-radio2')
      )).checked == true){
        $('#bottom-line').css({
          width:'124px',
          left: '150px',
          transition:'0s'
        });
      }
    }
  }
  slideForward() {
    (<HTMLInputElement>document.getElementById('detailCardNCradio2')).checked =
      true;
    (<HTMLInputElement>(
      document.getElementById('detailCardNavigationCarouselbtn-radio2')
    )).checked = true;
    $('.prev-btn.detailCardNavigationCarouselbtn img').attr(
      'src',
      'assets/prev5.png'
    );
    $('.next-btn.detailCardNavigationCarouselbtn img').attr(
      'src',
      'assets/next7.png'
    );
    let smallScreen = window.matchMedia('(max-width: 500px)');
    if (smallScreen.matches) {
      if((<HTMLInputElement>(document.getElementById('slide-controllers-slide-radio1')
      )).checked == true){
        $('#bottom-line').css({
          width:'160px',
          left: '-50px',
          transition:'0s'
        });
      }
      if((<HTMLInputElement>(document.getElementById('slide-controllers-slide-radio2')
      )).checked == true){
        $('#bottom-line').css({
          width:'124px',
          left: '73px',
          transition:'0s'
        });
      }
      if((<HTMLInputElement>(document.getElementById('slide-controllers-slide-radio3')
      )).checked == true){
        $('#bottom-line').css({
          width:'96px',
          left: '150px',
          transition:'0s'
        });
      }
    }
  }

  click_slide_controllers_slide(id: any) {
    console.log('clickk id=',id)
    // if(id==1){
    //   $('#businessImagesCarousel').css({
    //     'display':'flex'
    //   })
    // }
    if (id == 1) {
      console.log('id==1')

      let smallScreen = window.matchMedia('(max-width: 500px)');
      if (smallScreen.matches) {
        if (
          (<HTMLInputElement>document.getElementById('detailCardNCradio1'))
            .checked == true
        ) {
          $('#bottom-line').css({
          width:'160px',
            left: '0',
            transition:'1s'
          });
        }
        if (
          (<HTMLInputElement>document.getElementById('detailCardNCradio2'))
            .checked == true
        ) {
          $('#bottom-line').css({
          width:'160px',
            left: '-89px',
            transition:'1s'
          });
        }
      }
    }
    $('.no-map').html('');
    $('#dc-container').addClass('mb-5');

    if (id == 2) {
      console.log('id==2')
      let smallScreen = window.matchMedia('(max-width: 500px)');
      if (smallScreen.matches) {
        if (
          (<HTMLInputElement>document.getElementById('detailCardNCradio1'))
            .checked == true
        ) {
          $('#bottom-line').css({
          width:'124px',
            left: '160px',
            transition:'1s'
          });
        }
        if (
          (<HTMLInputElement>document.getElementById('detailCardNCradio2'))
            .checked == true
        ) {
          $('#bottom-line').css({
          width:'124px',
            left: '72px',
            transition:'1s'
          });
        }
      }
      // $('#slide2-container').css({
      //   'height':'auto'
      // })
      // $('#businessImagesCarousel').css({
      //   'display':'none'
      // })
      // this.height=700;
      // if (this.displayBusinessPhotosService.intervalID != -1) {
      //   clearInterval(this.displayBusinessPhotosService.intervalID);
      //   this.displayBusinessPhotosService.intervalID=-1;
      // }
      if (this.displayDetailsService.lat == -1) {
        $('#my-google-map').css({
          display: 'none',
        });
        $('.no-map').html('No map available');
      } else {
        this.center = {
          lat: this.displayDetailsService.lat,
          lng: this.displayDetailsService.lng,
        };
        this.marker = {
          position: {
            lat: this.displayDetailsService.lat,
            lng: this.displayDetailsService.lng,
          },
        };
        $('#my-google-map').css({
          display: 'inline-block',
        });
        $('.no-map').html('');
      }
    }

    if (id == 3) {
      console.log('id==3')

      let smallScreen = window.matchMedia('(max-width: 500px)');
      if (smallScreen.matches) {
        if (
          (<HTMLInputElement>document.getElementById('detailCardNCradio2'))
            .checked == true
        ) {
          $('#bottom-line').css({
          width:'96px',
            left: '197px',
            transition:'1s'
          });
        }
      }


      // $('#businessImagesCarousel').css({
      //   'display':'none'
      // })
      // if (this.displayBusinessPhotosService.intervalID != -1) {
      //   clearInterval(this.displayBusinessPhotosService.intervalID);
      //   this.displayBusinessPhotosService.intervalID=-1;
      // }
      $('#dc-container').removeClass('mb-5');
      $('#dc-container').css({
        'margin-bottom': '20px',
      });
    }

    (<HTMLInputElement>(
      document.getElementById('slide-controllers-slide-radio' + id)
    )).checked = true;
    // this.height=496;
  }

  renderCarousel() {
    this.zoom = 14;
  }

  clickCancelReservationButton() {
    let id = $('.cr-btn').attr('id');
    let index = this.reservationDataService.getReservationIndex(id);
    this.reservationDataService.removeReservationData(id);
    this.deleteBookingService.deleteBooking(index);
    alert('Reservation canceled!');
    $('.rn-btn').css({ display: 'inline-block' });
    $('.cr-btn').css({ display: 'none' });
  }

  submitReservationForm(formData: any, formDirective: FormGroupDirective) {
    this.submitted = true;
    if (this.reservationForm.invalid) {
      return;
    } else {
      this.clickSubmitReservationService.submitReservation(
        $('#businessNameOnReservationForm').html(),
        $('.rn-btn').attr('id')
      );
      $('.rn-btn').css({ display: 'none' });
      $('.cr-btn').css({ display: 'inline-block' });
      alert('Reservation created!');
      $('#modal-close-btn').trigger('click');
    }
    formDirective.resetForm();
    this.reservationForm.reset();
  }

  @HostListener('window:resize', ['$event'])
  onResize(event: any) {
    let smallScreen = window.matchMedia('(max-width: 500px)');
    if (smallScreen.matches) {
      this.width = 360;
      this.height = 531;

      $('.result-table-img').css({
        height: '50px',
        width: '50px',
        'object-fit': 'fill',
      });
      $('.rt-row').css({
        height: '70px',
        'font-size': '13px',
      });

      $('.lp').css({
        'padding-left': '11.5px',
      });
    }

    let largeScreen = window.matchMedia('(min-width: 500px)');
    if (largeScreen.matches) {
      this.width = 790;
      this.height = 496;

      $('.result-table-img').css({
        height: '98px',
        width: '98px',
        'object-fit': 'fill',
      });

      $('.rt-row').css({
        height: '116px',
        'font-size': '16px',
      });
      $('.lp').css({
        'padding-left': '24px',
      });
    }
  }
}
