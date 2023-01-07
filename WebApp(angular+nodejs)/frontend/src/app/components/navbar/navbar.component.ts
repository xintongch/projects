import { Component, OnInit } from '@angular/core';

import { HostListener } from '@angular/core';

@Component({
  selector: 'app-navbar',
  templateUrl: './navbar.component.html',
  styleUrls: ['./navbar.component.css'],
})
export class NavbarComponent implements OnInit {
  constructor() {}

  ngOnInit(): void {}

  @HostListener('window:resize', ['$event'])
  onResize(event: any) {

    let smallScreen = window.matchMedia('(max-width: 500px)');
    if (smallScreen.matches) {
      $('#header1, #header2').css({
        padding: '8.8px 8px',
        'font-size': '17.3px',
      });
    }

    let largeScreen = window.matchMedia('(min-width: 500px)');
    if (largeScreen.matches) {
      $('#header1, #header2').css({
        padding: '8px 8px',
        'font-size': '15px',
      });
    }
  }
}
