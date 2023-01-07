import { Injectable } from '@angular/core';
import { apidataService } from './api-data.service';
import { DisplayBusinessDetailsService } from './display-business-details.service';
import { DisplayBusinessPhotosService } from './display-business-photos.service';
import { ReservationDataService } from './reservation-data.service';

@Injectable({
  providedIn: 'root',
})
export class DisplayDetailsService {
  constructor(
    private apidataService: apidataService,
    private displayBusinessDetailsService: DisplayBusinessDetailsService,
    private displayBusinessPhotosService: DisplayBusinessPhotosService,
    private reservationDataService: ReservationDataService
  ) {}

  reviewPos = 1;
  lat = 0;
  lng = 0;
  reviewL = 0;
  currentReview = 0;


  addReviewSection(name: any, rating: any, text: any, time: any) {
    if (this.reviewPos == 1) {
      this.reviewPos = 2;
      if (this.currentReview == this.reviewL) {
        $('#slide3-container').append(
          "<div class='w-100 px-2 text-left border-bottom' style='background-color:rgb(246, 243, 243);border-radius:0px 0px 15px 15px;'><div class='pt-2' style='font-weight:500'>" +
            name +
            "</div><div class='mt-0' style=''>rating: " +
            rating +
            '/5' +
            "</div><div class='mt-3 lh-base' style=''>" +
            text +
            "</div><div class='mt-3 pb-4' style=''>" +
            time +
            '</div></div>'
        );
      } else {
        $('#slide3-container').append(
          "<div class='w-100 px-2 text-left border-bottom aligh-top' style='background-color:rgb(246, 243, 243);'><div class='pt-2' style='font-weight:500'>" +
            name +
            "</div><div class='mt-0' style=''>rating: " +
            rating +
            '/5' +
            "</div><div class='mt-3 lh-base' style=''>" +
            text +
            "</div><div class='mt-3 pb-4' style=''>" +
            time +
            '</div></div>'
        );
      }
    } else {
      this.reviewPos = 1;
      if (this.currentReview == this.reviewL) {
        $('#slide3-container').append(
          "<div class='w-100 px-2 text-left bg-white border-bottom' style='border-radius:0px 0px 15px 15px'><div class='pt-2' style='font-weight:500'>" +
            name +
            "</div><div class='mt-0' style=''>rating: " +
            rating +
            '/5' +
            "</div><div class='mt-3 lh-base' style=''>" +
            text +
            "</div><div class='mt-3 pb-4' style=''>" +
            time +
            '</div></div>'
        );
      } else {
        $('#slide3-container').append(
          "<div class='w-100 px-2 text-left bg-white border-bottom' style=''><div class='pt-2' style='font-weight:500'>" +
            name +
            "</div><div class='mt-0' style=''>rating: " +
            rating +
            '/5' +
            "</div><div class='mt-3 lh-base' style=''>" +
            text +
            "</div><div class='mt-3 pb-4' style=''>" +
            time +
            '</div></div>'
        );
      }
    }
  }

  displayReviews(id: any) {
    this.reviewL = 0;
    this.currentReview = 0;
    $('#slide3-container').children().remove();
    this.apidataService.yelpReviews(id).subscribe((reviews) => {
      this.reviewL = reviews.reviews.length;
      let reviewIndex=0;
      if(this.reviewL==0){
        $('#slide3-container').html('No review available');
      }else{
        reviews.reviews.forEach((review: any) => {
          reviewIndex+=1;
          if(reviewIndex>3){
            return;
          }
          this.currentReview++;
          let time = review.time_created.split(' ')[0];
          this.addReviewSection(
            review.user.name,
            review.rating,
            review.text,
            time
          );
        });
      }

    });
    this.reviewPos = 1;
  }

  displayDetails(details: any) {
    // this.displayBusinessPhotosService.displayNew=false;
    if (this.displayBusinessPhotosService.intervalID != -1) {
      clearInterval(this.displayBusinessPhotosService.intervalID);
      this.displayBusinessPhotosService.intervalID=-1;
    }
    $('#click_slide_controllers_slide1').trigger('click');
    $('#businessNameOnDetailCard').html(details.name);

    this.displayBusinessDetailsService.displayBusinessDetails(details);
    this.displayBusinessPhotosService.displayBusinessPhotos(details);
    $('#businessNameOnReservationForm').html(details.name);
    if(!details.hasOwnProperty('coordinates') || !details.coordinates.hasOwnProperty('latitude') || !details.coordinates.hasOwnProperty('longitude')){
      this.lat=-1;
      this.lng=-1;
    }else{
      this.lat = details.coordinates.latitude;
      this.lng = details.coordinates.longitude;
    }
    this.displayReviews(details.id);
    (<HTMLInputElement>document.getElementById('detailCardNCradio1')).checked =
      true;
    (<HTMLInputElement>(
      document.getElementById('detailCardNavigationCarouselbtn-radio1')
    )).checked = true;
    (<HTMLInputElement>(
      document.getElementById('slide-controllers-slide-radio1')
    )).checked = true;
    if (this.reservationDataService.checkReservationData(details.id)) {
      $('.rn-btn').css({ display: 'none' });
      $('.cr-btn').css({ display: 'inline-block' });
    } else {
      $('.rn-btn').css({ display: 'inline-block' });
      $('.cr-btn').css({ display: 'none' });
    }
    $('#dc-container').attr('style', 'display:inline-block');
    $('#rt-container').attr('style', 'display:none');
  }
}
