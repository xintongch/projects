import { Component, OnInit } from '@angular/core';
import { ClickSubmitReservationService } from 'src/app/services/click-submit-reservation.service';
import { DeleteBookingService } from 'src/app/services/delete-booking.service';

import { HostListener } from '@angular/core';

@Component({
  selector: 'app-bookings',
  templateUrl: './bookings.component.html',
  styleUrls: ['./bookings.component.css'],
})
export class BookingsComponent implements OnInit {
  constructor(
    private clickSubmitReservationService: ClickSubmitReservationService,
    private deleteBookingService: DeleteBookingService
  ) {}

  clickDeleteImage(imgId: any) {
    alert('Reservation canceled!');
    let id = parseInt(imgId.charAt(imgId.length - 1));
    this.deleteBookingService.deleteBooking(id);
  }

  ngOnInit(): void {
    // window.localStorage.clear()
    $('#header2').css({
      border: '2px black solid',
    });
    $('#header1').css({
      border: 'none',
    });

    if (
      window.localStorage.getItem('bookings') == null ||
      window.localStorage.getItem('bookings') == '[]'
    ) {
      $('#no-booking').attr('style', 'display:inline-block');
      $('#br-container').attr('style', 'display:none');
    } else {
      $('#no-booking').attr('style', 'display:none');
      $('#bt-container table tbody').append(
        window.localStorage.bookings_table_html_text
      );
      $('#br-container').attr('style', 'display:inline-block');
    }

    $('.hover-gray').hover(
      function () {
        $(this).css({
          'background-color': 'rgb(242, 239, 239)',
        });
      },
      function () {
        $(this).css({
          'background-color': 'white',
        });
      }
    );

    let self = this;
    $('img').click(function () {
      self.clickDeleteImage(this.id);
    });

    let smallScreen = window.matchMedia('(max-width: 500px)');
    if (smallScreen.matches) {
      $('#b-table tbody tr').css({
        height: '65px',
      });
    }

    let largeScreen = window.matchMedia('(min-width: 500px)');
    if (largeScreen.matches) {
      $('#b-table tbody tr').css({
        height: '41px',
      });
    }
  }

  @HostListener('window:resize', ['$event'])
  onResize(event: any) {

    let smallScreen = window.matchMedia('(max-width: 576px)');
    if (smallScreen.matches) {
      $('.h-tb').css({
        height: '65px',
      });
      $('#b-table').css({
        width: '420px',
      });
      $('.th-b').css({
        'vertical-align': 'bottom',
        'padding-bottom': '10px',
      });
      $('#b-table tbody tr').css({
        height: '65px',
      });
      $('#bt-h1').css({
        width: '6%',
      });
      $('#bt-h2').css({
        width: '24%',
      });
      $('#bt-h3').css({
        width: '13%',
      });
      $('#bt-h4').css({
        width: '17%',
      });
      $('#bt-h5').css({
        width: '31%',
      });
    }

    let largeScreen = window.matchMedia('(min-width: 576px)');
    if (largeScreen.matches) {
      $('#b-table').css({
        width: '100%',
      });
      $('.th-b').css({
        'vertical-align': 'middle',
        'padding-bottom':'0px'
      });
      $('.h-tb').css({
        height: '41px',
      });
      $('#b-table tbody tr').css({
        height: '41px',
      });
      $('#bt-h1').css({
        width: '5.4%',
      });
      $('#bt-h2').css({
        width: '28.2%',
      });
      $('#bt-h3').css({
        width: '20%',
      });
      $('#bt-h4').css({
        width: '10.7%',
      });
      $('#bt-h5').css({
        width: '31%',
      });
    }
  }
}
