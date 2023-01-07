import { TestBed } from '@angular/core/testing';

import { DisplayDetailsService } from './display-details.service';

describe('DisplayDetailsService', () => {
  let service: DisplayDetailsService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(DisplayDetailsService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
