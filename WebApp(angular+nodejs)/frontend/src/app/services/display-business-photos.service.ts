import { Injectable } from '@angular/core';
import { count, interval } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class DisplayBusinessPhotosService {
  displayedPhotoNumber: any=3;
  currentBusinessImageIndex = 1;
  cycle = 0;
  clickArrow = 0;
  intervalID = -1;
  displayNew=false;

  counter = 0;
  counter1 = 0;
  counter2 = 0;
  counter0 = 0;

  rotateStarted=0;

  constructor() {}

  rotateImage() {
    // if(this.intervalID==-1){
    this.counter = this.currentBusinessImageIndex;
    this.counter1 = this.currentBusinessImageIndex;
    this.counter2 = this.currentBusinessImageIndex;
    this.counter0 = this.currentBusinessImageIndex;

    var self = this;
      this.intervalID = window.setInterval(function () {
        console.log('set interval')
        if (self.clickArrow == 1) {
          if(self.rotateStarted==0){
            self.rotateStarted=1
            self.counter2=self.currentBusinessImageIndex;
            self.counter=self.currentBusinessImageIndex;
          }
          self.rotateStarted=1;
          self.counter1 = self.currentBusinessImageIndex;
          self.counter1++;
          if (self.counter1 > self.displayedPhotoNumber) {
            self.counter1 = 1;
          }
  
          if (self.counter1 == self.counter2) {
            if (self.cycle == 0) {
              self.counter = self.counter1;
              (<HTMLInputElement>(
                document.getElementById(
                  'businessImagesCarousel-radio' + self.counter
                )
              )).checked = true;
              self.currentBusinessImageIndex = self.counter;
            }
            self.cycle = 0;
          } else {
            self.counter2 = self.currentBusinessImageIndex;
            self.cycle = 0;
          }
          self.counter2++;
          if (self.counter2 > self.displayedPhotoNumber) {
            self.counter2 = 1;
          }
        }
      }, 5000);
    // }
  }

  prev() {

    this.currentBusinessImageIndex--;
    if (this.currentBusinessImageIndex < 1) {
      this.currentBusinessImageIndex = this.displayedPhotoNumber;
    }

    (<HTMLInputElement>(
      document.getElementById(
        'businessImagesCarousel-radio' + this.currentBusinessImageIndex
      )
    )).checked = true;
    this.cycle = 1;
    this.clickArrow = 1;
    if(this.intervalID==-1){
      this.rotateImage();
    }
  }

  next() {

    this.currentBusinessImageIndex++;
    if (this.currentBusinessImageIndex > this.displayedPhotoNumber) {
      this.currentBusinessImageIndex = 1;
    }

    (<HTMLInputElement>(
      document.getElementById(
        'businessImagesCarousel-radio' + this.currentBusinessImageIndex
      )
    )).checked = true;
    this.cycle = 1;
    this.clickArrow = 1;
    if(this.intervalID==-1){
      this.rotateImage();
    }
  }

  displayBusinessPhotos(details: any) {
    this.displayNew=true;
    $('.no-business-photo').html('')
    this.clickArrow = 0;
    this.rotateStarted=0;
    // clearInterval(this.intervalID);
    // if (this.intervalID != -1) {
    //   clearInterval(this.intervalID);
    //   this.intervalID=-1;
    // }
    (<HTMLInputElement>(
      document.getElementById('businessImagesCarousel-radio1')
    )).checked = true;

    this.currentBusinessImageIndex = 1;
    this.displayedPhotoNumber = 0;
    if (details.hasOwnProperty('photos') && details.photos != null) {
      this.displayedPhotoNumber = Math.min(3, details.photos.length);
      for (let i = 1; i <= this.displayedPhotoNumber; i++) {
        $('#businessimage' + i).attr('src', details.photos[i - 1]);
      }
    }
    if (this.displayedPhotoNumber == 0) {
      $('#businessImagesCarousel').css({ display: 'none' });
      $('.no-business-photo').html('no-photo')
    } else {
      $('#businessImagesCarousel').css({ display: 'flex' });
      $('.no-business-photo').html('')
    }

    this.rotateImage();
  }
}
