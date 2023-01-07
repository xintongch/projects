import { Injectable } from '@angular/core';
import * as e from 'express';

@Injectable({
  providedIn: 'root',
})
export class DisplayBusinessDetailsService {
  constructor() {}

  currentPos = 0;
  reviewPos = 1;

  addRowCol(title: any, content: any) {

    if (this.currentPos == 0) {
      this.currentPos = 1;
      $('#slide1-table').append("<div class='row businessDetailsRow'></div>");
    } else {
      this.currentPos = 0;
    }
    let lastRow = $('#slide1-table > div:last-child');
    if (content == 'Open Now') {
      lastRow.append(
        "<div class='col-sm-6 mt-md-3 pt-md-1 mt-4 pt-sm-0'><div class='' style='font-size:19.5px;font-weight:500'>" +
          title +
          "</div><div class='mt-1' style='color:green;font-size: 15.7px'>" +
          content +
          '</div></div>'
      );
    } else if (content == 'Closed') {
      lastRow.append(
        "<div class='col-sm-6 mt-md-3 pt-md-1 mt-4 pt-sm-0'><div class='' style='font-size:19.5px;font-weight:500'>" +
          title +
          "</div><div class='mt-1' style='color:red;font-size:15.7px'>" +
          content +
          '</div></div>'
      );
    } else {
      lastRow.append(
        "<div class='col-sm-6 mt-md-3 pt-md-1 mt-4 pt-sm-0'><div class='' style='font-size:19.5px;font-weight:500'>" +
          title +
          "</div><div class='mt-1' style='font-size:15.7px'>" +
          content +
          '</div></div>'
      );
    }
  }

  displayBusinessDetails(details: any) {
    this.currentPos=0;
    $('.rn-btn').attr('id', details.id);
    $('.cr-btn').attr('id', details.id);

    $('#slide1-container').children().remove();
    $('#slide1-container').append("<div id='slide1-table' class='mt-3'></div>");

    if (
      details.hasOwnProperty('location') &&
      details.location.hasOwnProperty('display_address') &&
      details.location.display_address.length != 0
    ) {
      this.addRowCol(
        'Address',
        details.location.display_address[0] +
          ' ' +
          details.location.display_address[1]
      );
    }

    if (
      details.hasOwnProperty('categories') &&
      details.categories.length != 0
    ) {
      let cat = '';
      details.categories.forEach((c: any, i: any) => {
        cat += c.title;
        cat += ' | ';
      });
      cat = cat.slice(0, -3);
      this.addRowCol('Category', cat);
    }

    if (
      details.hasOwnProperty('display_phone') &&
      details.display_phone != null &&
      details.display_phone.length != 0
    ) {
      this.addRowCol('Phone', details.display_phone);
    } else if (
      details.hasOwnProperty('phone') &&
      details.phone != null &&
      details.phone.length != 0
    ) {
      this.addRowCol('Phone', details.phone);
    }

    if (details.hasOwnProperty('price') && details.price != null) {
      this.addRowCol('Price range', details.price);
    }

    if (
      details.hasOwnProperty('hours') &&
      details.hours.length != 0 &&
      details.hours[0].hasOwnProperty('is_open_now') &&
      details.hours[0].is_open_now != null
    ) {
      let is_open = details.hours[0].is_open_now;
      if (is_open == false) {
        this.addRowCol('Status', 'Closed');
      } else {
        this.addRowCol('Status', 'Open Now');
      }
    }

    if (this.currentPos == 0) {
      $('#slide1-table').append("<div class='row'></div>");
      this.currentPos = 1;
    } else {
      this.currentPos = 0;
    }
    let lastRow = $('#slide1-table > div:last-child');
    lastRow.append(
      "<div class='col-sm-6 mt-lg-3 pt-lg-1 mt-4 pt-sm-0'><div class='' style='font-size:19.5px;font-weight:500'>Visit yelp for more</div><div class='mt-1' style='font-size:15.7px'><a href='" +
        details.url +
        "' style='text-decoration: underline' target='_blank'>Business link</a></div></div>"
    );

    let twitterLink =
      'https://twitter.com/intent/tweet?text=' +
      encodeURIComponent('Check ' + details.name + ' on Yelp.\n' + details.url.split('&')[0]);
    let facebokLink = 'https://www.facebook.com/sharer/sharer.php?u='+
    encodeURIComponent(details.url);

    $('.twitter-link').attr('href', twitterLink);
    $('.facebook-link').attr('href', facebokLink);
  }
}
